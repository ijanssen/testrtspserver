#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>

int main(int argc, char* argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  gchar* str = NULL;

  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new();
  g_object_set(
      server,
      "service",
      "8500",
      "address",
      "0.0.0.0",
      NULL);

  /* get the mount points for this server, every server has a default object
   * that be used to map uri mount points to media factories */
  mounts = gst_rtsp_server_get_mount_points (server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new();

  if (argc < 2) {
      str = g_strdup_printf("( videotestsrc is-live=1 ! video/x-raw,width=1280,height=768,framerate=25/1 ! x264enc ! rtph264pay name=pay0 pt=96 )");
  } else {
      str = g_strdup_printf("( filesrc location=%s ! qtdemux name=d ! rtph264pay name=pay0 pt=96 )", argv[1]);
  }
  gst_rtsp_media_factory_set_launch(factory, str);
  //  gst_rtsp_media_factory_set_launch(factory,
  //      "( videotestsrc is-live=1 ! video/x-raw,width=1280,height=768,framerate=25/1 ! x264enc ! rtph264pay name=pay0 pt=96 )");
  //  gst_rtsp_media_factory_set_launch(factory,
  //      "( filesrc location=a.mp4 ! qtdemux name=d ! rtph264pay name=pay0 pt=96 )");

  gst_rtsp_media_factory_set_shared(factory, TRUE);

  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory(mounts, "/stream", factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref (mounts);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);
  g_free(str);

  /* start serving */
  g_print("stream ready at rtsp://0.0.0.0:8500/stream\n");
  g_main_loop_run (loop);

  return 0;
}
