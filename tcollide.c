#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#define arraysize(x)  (sizeof(x) / sizeof((x)[0]))

void add_context(const gchar *style, GtkWidget *widget);
void packboxes();
void createwindow(const gchar *style);
void create_entries();
void set_spacing(GtkWidget *widget, int colspace, int rowspace);
void createlabels(GtkWidget *label, gchar *labeltext[], const gchar *style);
void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns, const gchar *style[], int stylenumber);
void button_connect_callback(GtkWidget *button, void *button_callback);
void createradiobuttons(gchar *radiolabels[], void *radiocallback[], const gchar *style, int arraysize);
void isSHA256();
void isMD5();
void createfilechoosers();
void createrainbowtables();
void readfiles();
// compile with gcc -Wall -g tcollide.c -o tcollide `pkg-config --cflags --libs gtk+-3.0` -lcrypto


struct widgets
{
	GtkWidget *window;
	GtkWidget *vbox; //widget for the vertical box
	GtkWidget *hbox;
	GtkWidget *buttonbox;
	GtkWidget *radiobox;
	GtkWidget *chooserbox;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *filechoosers;
	GtkWidget *rootbutton;
	GtkWidget *connectwindow;
	GtkWidget *connectinglabel;
	GtkWidget *chooservbox;
}gwidget;

struct variables
{
	char *inputfile1;
	char *inputfile2;
	unsigned char *file1read;
	unsigned char *file2read;
	char *hashfile1;
	char *hashfile2;
	unsigned int algorithm;
	size_t arraylen;
	char *filenames[100];
	int placement;
}location;

int main(int argc, char *argv [])
{
const gchar *style[] = {"window_style", "label_style", "button_style"};
gchar *labeltext[] = {"File 1\n","File 2\n", "Output File\n", "Rainbow Table File\n"};
gchar *buttonlabels[] = {"Collide", "Create Rainbow Tables"};
gchar *radiolabels[] = {"MD5", "SHA256"};
gchar *chooserlabels [] = {"Open", "Open", "Open", "Open"};
void *choosercallbacks[] = {createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers};
void *radiocallback[] = {isSHA256, isMD5};
void *buttoncallbacks[] = {readfiles, createrainbowtables};
location.arraylen = arraysize(labeltext);
size_t rlabels_size = arraysize(radiolabels);
size_t buttonarr_size = arraysize(buttonlabels);
gtk_init(&argc, &argv); //starting gtk 

createwindow(style[0]);

gwidget.buttonbox = gtk_grid_new(); 
gwidget.chooserbox = gtk_grid_new();
createlabels(gwidget.label, labeltext, style[1]);
createsinglesizegrid(gwidget.buttonbox, buttonlabels, buttoncallbacks, 1, buttonarr_size, style, 2);
createsinglesizegrid(gwidget.chooserbox, chooserlabels, choosercallbacks, 4,1, style, 2);
set_spacing(gwidget.chooserbox, 4, 4);
set_spacing(gwidget.buttonbox, 4, 4);
createradiobuttons(radiolabels,radiocallback, style[1], rlabels_size);
packboxes();

g_signal_connect(G_OBJECT(gwidget.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

gtk_widget_show_all(gwidget.window); //shows all widgets 

gtk_main();//gtk main, this is the main loop of GTK
}

void add_context(const gchar *style, GtkWidget *widget)
{
	GtkCssProvider* Provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "Styles.css", NULL);
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(context, style);
	gtk_style_context_add_provider (context,GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void createwindow(const gchar *style)
{
	gwidget.window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //creates toplevel window
	gtk_window_set_title(GTK_WINDOW(gwidget.window), "TCollide"); //sets a window title 
	gtk_window_set_position(GTK_WINDOW(gwidget.window), GTK_WIN_POS_CENTER); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(gwidget.window), 5); //sets the border size of the window
	add_context(style, gwidget.window);
}

void packboxes()
{
	gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gtk_container_add(GTK_CONTAINER(gwidget.window), gwidget.vbox); //adds the vbox to the window 
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.radiobox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.grid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
}

void createlabels(GtkWidget *label, gchar *labeltext[], const gchar *style)
{
	gwidget.grid = gtk_grid_new();

	for (int i=0; i<location.arraylen; i++)
	{
		for(int j = 0; j<1; j++)
		{	
			label = gtk_label_new(labeltext[i]);
			add_context(style, label);
			gtk_grid_attach(GTK_GRID(gwidget.grid), label, j, i, 1, 1); //sets the defaults for creating each table button
		}
	}
}

void createradiobuttons(gchar *radiolabels[], void *radiocallback[], const gchar *style, int arraysize)
{
	gwidget.radiobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gwidget.rootbutton = gtk_radio_button_new_with_label(NULL, radiolabels[0]);
	add_context(style, gwidget.rootbutton);
	button_connect_callback(gwidget.rootbutton, radiocallback[0]);
	gtk_box_pack_start(GTK_BOX(gwidget.radiobox), gwidget.rootbutton, FALSE, FALSE, 0); //packs the display into the vbox
	GtkWidget *labels;
	for (int i = 1; i<arraysize; i++)
	{
	labels = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(gwidget.rootbutton), radiolabels[i]);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(labels), FALSE);
	add_context(style, labels);
	button_connect_callback(labels, radiocallback[i]);
	gtk_box_pack_start(GTK_BOX(gwidget.radiobox), labels, FALSE, FALSE, 0); //packs the display into the vbox
	}
}

void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns, const gchar *style[], int stylenumber)
{
int pos = 0;
GtkWidget *button; //widget for the buttons 
	for (int i=0; i < rows; i++) //for loop for the rows
	{
		for (int j=0; j < columns; j++) //for loop for the columns
		{
		button = gtk_button_new_with_label(labels[pos]); //sets each button label to the respective button 
		if(strcmp(labels[pos], "Open")==0)
		{
			location.placement++;
			
		}
		button_connect_callback(button, callback[pos]); //attaches the button to the respective callback
		gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1); //sets the defaults for creating each table button
		gtk_widget_set_size_request(button, 70, 30); //sets the size of the buttons
		add_context(style[stylenumber], button);
		pos++; //changes the position 
		}
	}
}

void createfilechoosers()
{
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
gint res;
GtkWindow *new_window;
new_window = gtk_window_new(GTK_WINDOW_POPUP);
gwidget.filechoosers = gtk_file_chooser_dialog_new ("Open File", new_window, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

res = gtk_dialog_run (GTK_DIALOG (gwidget.filechoosers));
if (res == GTK_RESPONSE_ACCEPT)
  {
   GtkFileChooser *chooser = GTK_FILE_CHOOSER (gwidget.filechoosers);
   if(location.placement == 4)
   	{
   		location.placement = 1;
   	}
    location.filenames[location.placement] =  gtk_file_chooser_get_filename (chooser);
    location.placement++;
   // g_free(location.filenames[location.placement]);
  }

gtk_widget_destroy (gwidget.filechoosers);
}

void button_connect_callback(GtkWidget *button, void *button_callback)
{
	g_signal_connect(button, "clicked", G_CALLBACK(button_callback), NULL);
}

void set_spacing(GtkWidget *widget, int colspace, int rowspace)
{
	gtk_grid_set_column_spacing(GTK_GRID(widget), colspace);
	gtk_grid_set_row_spacing(GTK_GRID(widget), rowspace);
}

void readfiles()
{
  	FILE *file1 = fopen (location.filenames[1], "r");
  	FILE *file2 = fopen (location.filenames[2], "r");
  	FILE *hash1 = fopen("hashfile1.txt", "r+");
  	FILE *hash2 = fopen("hashfile2.txt", "r+");
  	FILE *resultfile = fopen(location.filenames[3], "w");
	char hash1line[128];
	char hash2line[128];
	char plaintext1line[128];
	char plaintext2line[128];
	unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    unsigned char md5_digest[MD5_DIGEST_LENGTH];
    unsigned char newline[] = {" "};

  	if(file1 != NULL && file2 != NULL)
  	{
  		while (1)
	  		{
	  			if (fgets(plaintext1line, sizeof plaintext1line, file1)!= NULL && fgets(plaintext2line,
	  		 sizeof plaintext2line, file2)!= NULL)
	  			{
			  			//fscanf(file1, "%s[^\n]", plaintext1line);
			  			//fscanf(file2, "%s[^\n]", plaintext2line);
			  			location.file1read = (unsigned char *)plaintext1line;
			  			location.file2read = (unsigned char *)plaintext2line;
			  			if (location.algorithm == 0)
			  			{
			  				MD5(location.file1read, sizeof location.file1read, md5_digest);
			  				
			  				for (int i = 0; i < sizeof md5_digest; i++)
			  				{
			  					fprintf(hash1, "%x", md5_digest[i]);
			  				}
			  				fprintf(hash1, "%s\n", newline);

			  				MD5(location.file2read, sizeof location.file2read, md5_digest);

			  				for (int j = 0; j < sizeof md5_digest; j++)
			  				{
			  					fprintf(hash2, "%x", md5_digest[j]);
			  				}
			  				fprintf(hash2, "%s\n", newline);
			  			}

			  			else if (location.algorithm == 1)
			  			{
			  				SHA256(location.file1read, sizeof location.file1read, sha256_digest);
			  				for (int i = 0; i < sizeof location.file1read; i++)
			  				{
			  					fprintf(hash1, "%x", sha256_digest[i]);
			  				}
			  				fprintf(hash1, "%s\n", newline);
			  				SHA256(location.file2read, sizeof location.file2read, sha256_digest);

			  				for (int i = 0; i < sizeof location.file2read; i++)
			  				{
			  					fprintf(hash2, "%x", sha256_digest[i]);
			  				}
			  				fprintf(hash2, "%s\n", newline);
			  			}
  				}
  			else 
  			{
  				   break;
  			}
  			}
  		}
  		fclose(hash1);
  		fclose(hash2);
  		hash1 = fopen("hashfile1.txt", "r+");
  		hash2 = fopen("hashfile2.txt", "r+");
  		rewind(hash1);
  		rewind(hash2);
  			for(int i = 0; fgets(hash1line, sizeof hash1line, hash1) !=NULL; i++)
					{
						rewind(hash2);
						for (int j = 0; fgets(hash2line, sizeof hash2line, hash2) !=NULL; j++)
						{
							if(strcmp(hash1line, hash2line) == 0 )
		  					{
				  				fprintf(resultfile, "collision found on line %d of %s %s\ncollision found on %d of %s %s\n", 
						  		i, location.hashfile1, hash1line, j, location.hashfile2, hash2line);
		  					}
  						}
					}
  		fclose(resultfile);
  } 	

void createrainbowtables()
{
	FILE *file1 = fopen (location.filenames[1], "r");
	FILE *rainbow = fopen(location.filenames[4], "w");
	char plaintext1line[128];
	unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    unsigned char md5_digest[MD5_DIGEST_LENGTH];

	if (file1 != NULL)
	{
		while(1)
		{
			if (fgets(plaintext1line, sizeof plaintext1line, file1)!= NULL)
			{
				location.file1read = (unsigned char *)plaintext1line;
				if(location.algorithm == 0)
				{
					MD5(location.file1read, sizeof location.file1read, md5_digest);

					for (int i = 0; i < sizeof md5_digest; i++)
			  				{
			  					fprintf(rainbow, "%x", md5_digest[i]);
			  				}
			  				fprintf(rainbow, " %s\n", plaintext1line);
				}
				else if(location.algorithm == 1)
				{
					SHA256(location.file1read, sizeof location.file1read, sha256_digest);

					for (int i = 0; i < sizeof sha256_digest; i++)
			  				{
			  					fprintf(rainbow, "%x", sha256_digest[i]);
			  				}
			  				fprintf(rainbow, " %s\n", plaintext1line);
				}
			}
			else 
			{
				break;
			}

		}
		fclose(rainbow);
	}

}

void isSHA256()
{
 location.algorithm = 1;
   printf("%s\n",location.filenames[1]);
}
void isMD5()
{
 location.algorithm = 0;
}

void cancel()
{
exit(1);
}