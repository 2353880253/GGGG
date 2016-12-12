#include <gst/gst.h>
#include <glib.h>
#include <assert.h>
#include <unistd.h>

int Count = 0;

/**********************************************/
 typedef struct node {
         unsigned char elem;
         struct node *next;
 } Node, *List;


/**********************************************/

static void print_one_tag (const GstTagList * list, const gchar * tag, gpointer user_data)
{
  int i, num;

  num = gst_tag_list_get_tag_size (list, tag);
  for (i = 0; i < num; ++i) {
    const GValue *val;

    /* Note: when looking for specific tags, use the g_tag_list_get_xyz() API,
     * we only use the GValue approach here because it is more generic */
    val = gst_tag_list_get_value_index (list, tag, i);
    if (G_VALUE_HOLDS_STRING (val)) {
      g_print ("\t%20s : %s\n", tag, g_value_get_string (val));
    } else if (G_VALUE_HOLDS_UINT (val)) {
      g_print ("\t%20s : %u\n", tag, g_value_get_uint (val));
    } else if (G_VALUE_HOLDS_DOUBLE (val)) {
      g_print ("\t%20s : %g\n", tag, g_value_get_double (val));
    } else if (G_VALUE_HOLDS_BOOLEAN (val)) {
      g_print ("\t%20s : %s\n", tag,
          (g_value_get_boolean (val)) ? "true" : "false");
    } else if (GST_VALUE_HOLDS_BUFFER (val)) {
      g_print ("\t%20s : buffer of size %u\n", tag,
          GST_BUFFER_SIZE (gst_value_get_buffer (val)));
    } else if (GST_VALUE_HOLDS_DATE (val)) {
      g_print ("\t%20s : date (year=%u,...)\n", tag,
          g_date_get_year (gst_value_get_date (val)));
    } else {
      g_print ("\t%20s : tag of type '%s'\n", tag, G_VALUE_TYPE_NAME (val));
    }
  }
}

static gboolean buffering = FALSE;

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_TAG:
        {
            GstTagList *tags = NULL;
            gst_message_parse_tag (msg, &tags);
            g_print ("Got tags from element %s:\n", GST_OBJECT_NAME (GST_MESSAGE_SRC(msg)));
                gst_tag_list_foreach (tags, print_one_tag, NULL);
        }
        break;
    case GST_MESSAGE_NEW_CLOCK:
        g_print ("Get NEW_CLOCK MESSAGE from %s \n", GST_OBJECT_NAME (GST_MESSAGE_SRC(msg)));
        break;
    case GST_MESSAGE_BUFFERING:
        {
            gint percent;

            gst_message_parse_buffering (msg, &percent);
            g_print("buffering %d ...\r",percent);
        }
      break;
    default:
      break;
  }

  return TRUE;
}


static gboolean cb_print_position (GstElement *pipeline)
{
  gint64 pos, len;
  GstFormat format = GST_FORMAT_TIME;

  if (gst_element_query_position (pipeline, &format, &pos)
    && gst_element_query_duration (pipeline, &format, &len)) {
    g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));
  }
  /********************add file Save Code*********************************/
/*          //add  file Save Code //Change file_name to argv[1]
          char file_name[255] = "/home/user/Clock/123.h264"; //source path
          List head = (List)malloc(sizeof(Node));
          List p, q;
          int c;
          FILE *fp;
          if(head == NULL) {
                  fprintf(stderr, "malloc error!\n");
                  exit(1);
          }
          head->next = NULL;
          q = head;
          if((fp=fopen(file_name, "rb")) == NULL) {
                  fprintf(stderr, "打开文件失败!\n");
                  exit(1);
          }
          while((c=fgetc(fp)) != EOF) {
                 //while(fread(&c,sizeof(unsigned char), 1, fp)) {
                  p = (List)malloc(sizeof(Node));
                  if(p == NULL) {
                          fprintf(stderr, "malloc error!\n");
                          p = head;
                          while(p != NULL) {     //p=p->next
                                  q = p->next;
                                  free(p);
                                  p = q;
                          }
                          exit(1);
                  }
                  p->elem = c;               //if(p!=NULL) q = p;
                  p->next = NULL;
                  q->next = p;
                  q = p;
          }
          fclose(fp);
*/
/*******************************Fractional storage******************/  
/*
	 Count = Count++;
	   char filename[30];
	   sprintf(filename,"/home/user/ARMClock/Copy%d.h264",Count);
	   if(fopen(filename,"at+") == NULL)
	   {
	          printf("can't open the disk");
		  return 1;
	   }




*/


/*********************************************************/
/*          p = head->next;
          while(p != NULL) {
                q = p->next;
                  fputc(p->elem, fp);
                  //fwrite(&p->elem, sizeof(unsigned char), 1, fp);
                  free(p);
                  p = q;
            }
            printf("复制完成..\n");
	    printf("复制完成%d.h264\n",Count);
            fclose(fp);
*/
  /******************************************************/
  /* call me again */
            return TRUE;
}




int main (int argc, char *argv[])
{

/**********ADD****************/
      GstElement *pipeline;
      GstElement *filesink;	
      GError *error = NULL;	
/*****************************/


  /* Check input arguments */
  if (argc != 1) {
    g_printerr ("Usage: %s <uri>\n", argv[0]);
    return -1;
  }

  gst_init(&argc,&argv);
  /* Set up the pipeline */


 /*********ADD***********/
/* pipeline = gst_parse_launch ("
v4l2src always-copy=false queue-size=12 num-buffers=2000 !
video/x-raw-yuv-strided,format=(fourcc)NV12,width=1920,height=1080,framerate=(fraction)30/1 !
omxbufferalloc numBuffers=12 !
omx_h264enc bitrate=5000000 !    
filesink name=my_filesink",&error);
    printf("Hello GST!\n");
*/
  pipeline = gst_parse_launch ("v4l2src always-copy=false queue-size=12 ! video/x-raw-yuv-strided,format=(fourcc)NV12,width=1920,height=1080,framerate=(fraction)30/1 ! omxbufferalloc numBuffers=12 ! omx_mdeiscaler name=d  d.src_00 ! video/x-raw-yuv, width=(int)720, height=(int)480 !gstperf!v4l2sink sync=true device=/dev/video3 userpointer=true  min-queued-bufs=2  d.src_01 ! queue ! omx_h264enc bitrate=5000000 !  filesink name=my_filesink", &error);




 /***********************/


 	   printf("I'm build pipline now!\n");
           filesink = gst_bin_get_by_name(GST_BIN(pipeline),"my_filesink");
 
            g_object_set(filesink,"location","1.h264",NULL);
 
            g_object_unref(filesink);
 
 
 
 
       printf("build pipeline complete!\n");



  assert(pipeline!=NULL);
  /* we set the input filename to the source element */
//  g_object_set (G_OBJECT (pipeline), "uri", argv[1], NULL);
  /* we add a message handler */

  GstBus* bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  GMainLoop* loop = g_main_loop_new(NULL,FALSE);
  guint bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);
  /* Set the pipeline to "playing" state*/
  g_print ("Now playing: %s\n", argv[1]);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  /* Iterate */
  g_timeout_add (2000, (GSourceFunc) cb_print_position, pipeline);//Change 200 become BIG 2000
  g_main_loop_run (loop);
  /* Out of the main loop, clean up nicely */
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);
  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
