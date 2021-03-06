#include "gtktemplate.h"
#include <openssl/md5.h>
#include <openssl/sha.h>

void packboxes();
void isSHA256();
void isMD5();
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
	GtkWidget *labelgrid;
	GtkWidget *filechoosers;
	GtkWidget *rootbutton;
	GtkWidget *connectwindow;
}gwidget;

struct filevar
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
}filevars;

int main(int argc, char *argv [])
{
gchar *labeltext[] = {"File 1\n","File 2\n", "Output File\n", "Rainbow Table File\n"};
gchar *buttonlabels[] = {"Collide", "Create Rainbow Tables"};
gchar *radiolabels[] = {"MD5", "SHA256"};
gchar *chooserlabels [] = {"Open", "Open", "Open", "Open"};
void *choosercallbacks[] = {createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers};
void *radiocallback[] = {isSHA256, isMD5};
void *buttoncallbacks[] = {readfiles, createrainbowtables};
filevars.arraylen = arraysize(labeltext);
size_t rlabels_size = arraysize(radiolabels);
size_t buttonarr_size = arraysize(buttonlabels);

location data;

data = (location){.pointer = {""}, .current = 0, .max = 4};

gtk_init(&argc, &argv); //starting gtk 

GtkWidget *window = createwindow("tcollide", GTK_WIN_POS_CENTER);
gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
gtk_container_add(GTK_CONTAINER(window), gwidget.vbox); //adds the vbox to the window 

gwidget.buttonbox = createsinglesizegrid(buttonlabels, buttoncallbacks, NULL, 1, buttonarr_size);
set_spacing(gwidget.buttonbox, 4, 4);

gwidget.chooserbox = createsinglesizegrid(chooserlabels, choosercallbacks, &data, 4,1);
set_spacing(gwidget.chooserbox, 4, 4);

gwidget.labelgrid = createlabels(labeltext, filevars.arraylen);
gwidget.radiobox = createradiobuttons(radiolabels,radiocallback, rlabels_size);
packboxes();

show_and_destroy(window); //shows all widgets, connects the callback for the window and starts gtkmain
}

void packboxes()
{
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.radiobox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.labelgrid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.grid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
}

void readfiles()
{
  	FILE *file1 = fopen (filevars.filenames[1], "r");
  	FILE *file2 = fopen (filevars.filenames[2], "r");
  	FILE *hash1 = fopen("hashfile1.txt", "r+");
  	FILE *hash2 = fopen("hashfile2.txt", "r+");
  	FILE *resultfile = fopen(filevars.filenames[3], "w");
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
			  			filevars.file1read = (unsigned char *)plaintext1line;
			  			filevars.file2read = (unsigned char *)plaintext2line;
			  			if (filevars.algorithm == 0)
			  			{
			  				MD5(filevars.file1read, sizeof filevars.file1read, md5_digest);
			  				
			  				for (int i = 0; i < sizeof md5_digest; i++)
			  				{
			  					fprintf(hash1, "%x", md5_digest[i]);
			  				}
			  				fprintf(hash1, "%s\n", newline);

			  				MD5(filevars.file2read, sizeof filevars.file2read, md5_digest);

			  				for (int j = 0; j < sizeof md5_digest; j++)
			  				{
			  					fprintf(hash2, "%x", md5_digest[j]);
			  				}
			  				fprintf(hash2, "%s\n", newline);
			  			}

			  			else if (filevars.algorithm == 1)
			  			{
			  				SHA256(filevars.file1read, sizeof filevars.file1read, sha256_digest);
			  				for (int i = 0; i < sizeof filevars.file1read; i++)
			  				{
			  					fprintf(hash1, "%x", sha256_digest[i]);
			  				}
			  				fprintf(hash1, "%s\n", newline);
			  				SHA256(filevars.file2read, sizeof filevars.file2read, sha256_digest);

			  				for (int i = 0; i < sizeof filevars.file2read; i++)
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
						  		i, filevars.hashfile1, hash1line, j, filevars.hashfile2, hash2line);
		  					}
  						}
					}
  		fclose(resultfile);
  } 	

void createrainbowtables()
{
	FILE *file1 = fopen (filevars.filenames[1], "r");
	FILE *rainbow = fopen(filevars.filenames[4], "w");
	char plaintext1line[128];
	unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    unsigned char md5_digest[MD5_DIGEST_LENGTH];

	if (file1 != NULL)
	{
		while(1)
		{
			if (fgets(plaintext1line, sizeof plaintext1line, file1)!= NULL)
			{
				filevars.file1read = (unsigned char *)plaintext1line;
				if(filevars.algorithm == 0)
				{
					MD5(filevars.file1read, sizeof filevars.file1read, md5_digest);

					for (int i = 0; i < sizeof md5_digest; i++)
			  				{
			  					fprintf(rainbow, "%x", md5_digest[i]);
			  				}
			  				fprintf(rainbow, " %s\n", plaintext1line);
				}
				else if(filevars.algorithm == 1)
				{
					SHA256(filevars.file1read, sizeof filevars.file1read, sha256_digest);

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
 filevars.algorithm = 1;
   printf("%s\n",filevars.filenames[1]);
}
void isMD5()
{
 filevars.algorithm = 0;
}

void cancel()
{
exit(1);
}
