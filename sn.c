#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <webkit/webkit.h>


#define DOWNLOAD_DIR "/tmp/tmp"


static void sn_destroy_client(GtkWidget *, gpointer);
static gboolean sn_do_download(WebKitWebView *, WebKitDownload *, gpointer);
static gboolean sn_download_request(WebKitWebView *, WebKitWebFrame *,
                                    WebKitNetworkRequest *, gchar *,
                                    WebKitWebPolicyDecision *, gpointer);
static void sn_new_client(const gchar *uri);
static gboolean sn_new_client_request(WebKitWebView *, WebKitWebFrame *,
                                      WebKitNetworkRequest *,
                                      WebKitWebNavigationAction *,
                                      WebKitWebPolicyDecision *, gpointer);
static void sn_title_changed(GObject *, GParamSpec *, gpointer);


static Window embed = 0;
static int clients = 0;
static double global_zoom = 1.0;


struct Client
{
	GtkWidget *win;
	GtkWidget *scroll;
	GtkWidget *web_view;
};


void
sn_destroy_client(GtkWidget *obj, gpointer data)
{
	struct Client *c = (struct Client *)data;

	(void)obj;

	webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(c->web_view));
	gtk_widget_destroy(c->web_view);
	gtk_widget_destroy(c->scroll);
	gtk_widget_destroy(c->win);
	free(c);

	clients--;
	if (clients == 0)
		gtk_main_quit();
}

gboolean
sn_do_download(WebKitWebView *web_view, WebKitDownload *download, gpointer data)
{
	const gchar *uri;

	(void)web_view;
	(void)data;

	uri = webkit_download_get_uri(download);
	if (fork() == 0)
	{
		chdir(DOWNLOAD_DIR);
		if (execlp("xterm", "xterm", "-hold", "-e", "wget", uri, NULL) == -1)
		{
			fprintf(stderr, "sn: exec'ing xterm for download");
			perror(" failed");
			exit(EXIT_FAILURE);
		}
	}

	return FALSE;
}

gboolean
sn_download_request(WebKitWebView *web_view, WebKitWebFrame *frame,
                    WebKitNetworkRequest *request, gchar *mime_type,
                    WebKitWebPolicyDecision *policy_decision,
                    gpointer data)
{
	(void)frame;
	(void)request;
	(void)data;

	if (!webkit_web_view_can_show_mime_type(web_view, mime_type))
	{
		webkit_web_policy_decision_download(policy_decision);
		return TRUE;
	}
	return FALSE;
}

void
sn_new_client(const gchar *uri)
{
	struct Client *c = malloc(sizeof(struct Client));
	if (!c)
	{
		fprintf(stderr, "sn: fatal: malloc failed\n");
		exit(EXIT_FAILURE);
	}

	if (embed == 0)
	{
		c->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	}
	else
	{
		c->win = gtk_plug_new(embed);
	}

	/* When using Gtk2, sn only shows a white area when run in suckless'
	 * tabbed. It appears we need to set a default window size for this
	 * to work. This is not needed when using Gtk3. */
	gtk_window_set_default_size(GTK_WINDOW(c->win), 1024, 768);

	g_signal_connect(G_OBJECT(c->win), "destroy", G_CALLBACK(sn_destroy_client),
	                 c);
	gtk_window_set_title(GTK_WINDOW(c->win), "sn");

	c->web_view = webkit_web_view_new();
	webkit_web_view_set_full_content_zoom(WEBKIT_WEB_VIEW(c->web_view), TRUE);
	webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(c->web_view), global_zoom);
	g_signal_connect(G_OBJECT(c->web_view), "notify::title",
	                 G_CALLBACK(sn_title_changed), c->win);
	g_signal_connect(G_OBJECT(c->web_view),
	                 "new-window-policy-decision-requested",
	                 G_CALLBACK(sn_new_client_request), NULL);
	g_signal_connect(G_OBJECT(c->web_view), "mime-type-policy-decision-requested",
	                 G_CALLBACK(sn_download_request), NULL);
	g_signal_connect(G_OBJECT(c->web_view), "download-requested",
	                 G_CALLBACK(sn_do_download), NULL);

	c->scroll = gtk_scrolled_window_new(NULL, NULL);

	gtk_container_add(GTK_CONTAINER(c->scroll), c->web_view);
	gtk_container_add(GTK_CONTAINER(c->win), c->scroll);

	gtk_widget_show_all(c->win);

	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(c->web_view), uri);

	clients++;
}

gboolean
sn_new_client_request(WebKitWebView *web_view, WebKitWebFrame *frame,
                      WebKitNetworkRequest *request,
                      WebKitWebNavigationAction *navigation_action,
                      WebKitWebPolicyDecision *policy_decision,
                      gpointer user_data)
{
	(void)web_view;
	(void)frame;
	(void)navigation_action;
	(void)user_data;

	webkit_web_policy_decision_ignore(policy_decision);
	sn_new_client(webkit_network_request_get_uri(request));

	return TRUE;
}

void
sn_title_changed(GObject *obj, GParamSpec *pspec, gpointer data)
{
	const gchar *t;
	WebKitWebView *view = WEBKIT_WEB_VIEW(obj);
	GtkWindow *win = GTK_WINDOW(data);

	(void)pspec;

	t = webkit_web_view_get_title(view);
	gtk_window_set_title(win, (t == NULL ? "sn" : t));
}

int
main(int argc, char **argv)
{
	int opt;

	gtk_init(&argc, &argv);

	while ((opt = getopt(argc, argv, "z:e:")) != -1)
	{
		switch (opt)
		{
			case 'z':
				global_zoom = atof(optarg);
				break;
			case 'e':
				embed = atol(optarg);
				break;
		}
	}

	if (optind >= argc)
	{
		fprintf(stderr, "Usage: sn [OPTIONS] <URI>\n");
		exit(EXIT_FAILURE);
	}

	sn_new_client(argv[optind]);
	gtk_main();
	exit(EXIT_SUCCESS);
}
