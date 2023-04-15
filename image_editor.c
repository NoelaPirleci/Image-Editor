/*Pirleci Noela-Elena, 315CA*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIXEL_RANGE 256

/* structura care retine detaliile legate de imagine*/
struct image {
   char magic_num[3];
   int width;
   int height;
   int max_value; 
   int x1, x2, y1, y2;
	int **pixel_ascii;
   unsigned char **pixel_binary;
};

/* functie pentru a incarca o imagine de tip PGM sau PPM 
dintr-un fisier in memorie */                             
void load_image (FILE *file, struct image *pic) {
    
   int i, j;
	/* citesc antetul imaginii */
   fscanf (file, "%s\n", pic -> magic_num);
   fscanf (file, "%d %d\n", &(pic -> width), &(pic -> height));
	fscanf (file, "%d\n", &(pic -> max_value));
   /* aloc memorie in care stochez pixelii imaginii */
   if ( pic -> magic_num[1] == '2' || pic -> magic_num[1] == '3') {
      pic -> pixel_ascii = (int **)malloc(pic -> height * sizeof(int *));
		if (pic -> pixel_ascii == NULL) {
      printf ("Memory not allocated");
      exit (EXIT_FAILURE);
   	}
		else {
			for (i = 0; i < pic -> height; i++) {
				if (pic -> magic_num[1] == '2') {
 					pic -> pixel_ascii[i] = (int *)malloc(pic -> width * sizeof(int));
				} else {
               // aloc memorie pentru cele 3 canale de culoare
					pic -> pixel_ascii[i] = (int *)malloc(pic -> width * 3 * sizeof(int));
				}
            // programare defensiva
				if (pic -> pixel_ascii[i] == NULL) {
					printf ("Memory not allocated\n");
					exit (EXIT_FAILURE);
				}
			}
			if (pic -> magic_num[1] == '2') {
				for (i = 0; i < pic -> height; i++) {
					for (j = 0; j < pic -> width; j++) {
						fscanf (file, "%d", &pic -> pixel_ascii[i][j]);
					}
				}
			}
			else if (pic -> magic_num[1] == '3') {
				for (i = 0; i < pic -> height; i++) {
					for (j = 0; j < pic -> width * 3; j++) {
						fscanf (file, "%d", &pic -> pixel_ascii[i][j]);
					}
				}
			}
		}
      
	}
	if (pic -> magic_num[1] == '5' || pic -> magic_num[1] == '6') {
		pic -> pixel_binary = (unsigned char **)malloc(pic -> height * sizeof(unsigned char *));
		if (pic -> pixel_binary == NULL) {
			printf ("Memory not allocated\n");
			exit(EXIT_FAILURE);
		} else {
			for (i = 0; i < pic -> height; i++) {
				if (pic -> magic_num[1] == '5') {
					pic -> pixel_binary[i] = (unsigned char *)malloc(pic -> width * sizeof(unsigned char));
				} else if (pic -> magic_num[1] == '6') {
               //aloc memorie pentru cele 3 canale de culoare
					pic -> pixel_binary[i] = (unsigned char *)malloc(pic -> width * 3 * sizeof(unsigned char));
				}
				if (pic -> pixel_binary[i] == NULL) {
					printf ("Memory not allocated\n");
					exit(EXIT_FAILURE);
				}	
			}
			if (pic -> magic_num[1] == '5') {
				for (i = 0; i < pic -> height; i++) {
					fread (pic -> pixel_binary[i], 1, pic -> width, file);
				}	
			}
			else if (pic -> magic_num[1] == '6') {
				for (i = 0; i < pic -> height; i++) {
					for (j = 0; j < pic -> width * 3; j++) {
						fread( &pic -> pixel_binary[i][j], sizeof(unsigned char), 1, file );
					}
				}
			}
		}
	}
}

/* functie care salveaza o functie in format ASCII */ 
void save_image_ascii (FILE *file_out, struct image *pic, char filename[20]) {

   int i, j;
   /* deschid fisierul de iesire */
   file_out = fopen(filename, "w");
   /* verificare deschidere fisier */
   if (file_out == NULL) {
      printf ("Error: Can't open file");
      exit(EXIT_FAILURE);
   }
   /* daca e incarcata o imagine ascii, doar o copiez in fisierul de iesire */
   if (pic -> magic_num[1] == '2' || pic -> magic_num[1] == '3') {
      fprintf (file_out, "%s\n", pic -> magic_num);
      fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
      fprintf (file_out, "%d\n", pic -> max_value);
      if (pic -> magic_num[1] == '2') {
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               fprintf (file_out, "%d ", pic -> pixel_ascii[i][j]);
            }
            fprintf (file_out, "\n");
         }
      }
      else if (pic -> magic_num[1] == '3') {
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width * 3; j++) {
               fprintf (file_out, "%d ", pic -> pixel_ascii[i][j]);
            }
            fprintf (file_out, "\n");
         }
      }
   }
   /* daca e incarcata o imagine de tip ppm, ii schimb magic number-ul si copiez pixelii 
   din format binar in format ascii*/
   else if (pic -> magic_num[1] == '5' || pic -> magic_num[1] == '6') {
      if (pic -> magic_num[1] == '5') {
         fprintf (file_out, "P2\n");
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               fprintf (file_out, "%d ", (int) pic -> pixel_binary[i][j]);
            }
            fprintf(file_out, "\n");
         }
      }
      else if (pic -> magic_num[1] == '6') {
         fprintf (file_out, "P3\n");
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width * 3; j++) {
               fprintf (file_out, "%d ", (int) pic ->pixel_binary[i][j]);
            }
            fprintf(file_out, "\n");
         }
      }
   }
   fclose(file_out);
}

/* functie care salveaza o imagine in format binar */  
void save_image (FILE *file_out, struct image *pic, char filename[20]) {
   int i, j;
   unsigned char **copy;
   file_out = fopen(filename, "w");
   if (file_out == NULL) {
      printf ("Error: Can't open file\n");
      exit(EXIT_FAILURE);
   }
   /* daca e imagine de tip ppm o copiez direct in fisierul de iesire */
   if (pic -> magic_num[1] == '5' || pic -> magic_num[1] == '6') {
      if (pic -> magic_num[1] == '5') {
         fprintf (file_out, "%s\n", pic -> magic_num);
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         for (i = 0; i < pic -> height; i++) {
            fwrite (pic -> pixel_binary[i], 1, pic -> width, file_out);
         }   
      }
      else if (pic -> magic_num[1] == '6') {
         fprintf (file_out, "%s\n", pic -> magic_num);
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         for (i = 0; i < pic -> height; i++) {
            fwrite (pic -> pixel_binary[i], 3, pic -> width, file_out);
         }
      }
   }
   /* altfel, daca e pgm, aloc o noua matrice in care fac conversia pixelilor*/
   else {
      copy = (unsigned char **)malloc(pic -> height * sizeof(unsigned char*));
      if (copy == NULL) {
         printf ("Memory not allocated");
         exit(-1);
      }
      if (pic -> magic_num[1] == '2') {
         for (i = 0; i < pic -> height; i++) {
            copy[i] = (unsigned char *)malloc(pic -> width * sizeof(unsigned char));
         
         if (copy[i] == NULL) {
            printf ("Memory not allocated\n");
            exit(-1);
         }
         }
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               // convertesc pixelii din format ascii in format binar
               copy[i][j] =(unsigned char)pic -> pixel_ascii[i][j];
            }
         }
         //schimb magic number-ul, pentru ca se schimba si formatul fisierului
         fprintf (file_out, "P5\n");
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         //copiez pixelii din matricea ajutatoare
         for (i = 0; i < pic -> height; i++) {
               fwrite (copy[i], 1, pic -> width, file_out); 
         }
         //eliberez matricea ajutatoare
         for (i = 0; i < pic -> height; i++) {
            free(copy[i]);
         }
         free(copy);
      }
      else {
         for (i = 0; i < pic -> height; i++) {
            //aloc pentru cele trei canale de culoare
            copy[i] = (unsigned char *)malloc(pic -> width * 3 * sizeof(unsigned char));
            if (copy[i] == NULL) {
               printf ("Memory not allocated");
               exit(-1);
            }
         }
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width * 3; j++) {
               //convertesc pixelii in format binar
               copy[i][j] = (unsigned char)pic -> pixel_ascii[i][j];
            }
         }
         //schimb magic number-ul din acelasi motiv ca mai sus 
         fprintf (file_out, "P6\n");
         fprintf (file_out, "%d %d\n", pic -> width, pic -> height);
         fprintf (file_out, "%d\n", pic -> max_value);
         //copiez pixelii din matricea ajutatoare in fisier
         for (i = 0; i < pic -> height; i++) {
               fwrite (copy[i], 1, pic -> width * 3, file_out);  
         }
         //eliberez matricea ajutatoare
         for (i = 0; i < pic -> height; i++) {
            free(copy[i]);
         }
         free(copy);
      }
   }
   fclose(file_out);
}

/* functie care selecteaza doar o parte din imagine */
void select_part_image (struct image *pic, int *x1, int *y1, int *x2, int *y2) {

   //variabila care ma ajuta sa fac interschimbarea
   int help;
   //4 if-uri care verifica toate conditiile posibile pentru x1, x2, y1, y2
   if (*x1 < *x2 && *y1 < *y2) {
      if (*x1 < 0 || *x1 > pic -> width || *y1 < 0 || *y1 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      else if (*x2 < 0 || *x2 > pic -> width || *y2 < 0 || *y2 > pic ->height) {
         printf ("Invalid set of coordinates\n");
      }
      else  {
         printf ("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
         //atribui valorile, caci ma ajuta pe viitor
         pic -> x1 = *x1;
         pic -> y1 = *y1;
         pic -> x2 = *x2;
         pic -> y2 = *y2;
      }
   }
   else if (*x1 > *x2 && *y1 > *y2) {
      if (*x2 < 0 || *x2 > pic -> width || *y2 < 0 || *y2 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      else if (*x1 < 0 || *x1 > pic -> width || *y1 < 0 || *y1 > pic ->height) {
         printf ("Invalid set of coordinates\n");
      }
      else {
         //fac interschimbarea intre x1 si x2, respectiv y1 si y2
         help = *x1;
         *x1 = *x2;
         *x2 = help;
         help = *y1;
         *y1 = *y2;
         *y2 = help;
         printf ("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
         pic -> x1 = *x1;
         pic -> y1 = *y1;
         pic -> x2 = *x2;
         pic -> y2 = *y2;
      }
   } 
   else if (*x1 < *x2 || *y1 > *y2) {
      if (*x1 < 0 || *x1 > pic -> width || *y2 < 0 || *y2 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      else if (*x2 < 0 || *x2 > pic -> width || *y1 < 0 || *y1 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      else if((*y1-*y2)!=0 &&(*x1-*x2)!=0) {
         //fac interschimbarea intre y1 si y2
         help = *y1;
         *y1 = *y2;
         *y2 = help;
         printf ("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
         pic -> x1 = *x1;
         pic -> y1 = *y1;
         pic -> x2 = *x2;
         pic -> y2 = *y2;
      }
       else
      {
         printf("Invalid set of coordinates\n");
      }
   }
   else if (*x1 > *x2 || *y1 < *y2) {
      if (*x2 < 0 || *x2 > pic -> width || *y1 < 0 || *y1 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      else if (*x1 < 0 || *x1 > pic -> width || *y2 < 0 || *y2 > pic -> height) {
         printf ("Invalid set of coordinates\n");
      }
      /*conditie extra, caci daca variabilele sunt egale, nu se poate
       face selectia*/
      else if((*y1-*y2)!=0 &&(*x1-*x2)!=0){
         //fac interschimbarea intre x1 si x2
         help = *x1;
         *x1 = *x2;
         *x2 = help;
         printf ("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
         pic -> x1 = *x1;
         pic -> y1 = *y1;
         pic -> x2 = *x2;
         pic -> y2 = *y2;
      }
      else
      {
         printf("Invalid set of coordinates\n");
      } 
   }
   else
   {  
      printf ("Invalid set of coordinates\n");
   }   
}

/* functie care selecteaza intreaga suprafata a unei imagini */
void select_all_image (struct image *pic) {

   int x_i = 0;
   int x_f = pic -> width;
   int y_i = 0;
   int y_f = pic -> height;
   printf("Selected ALL\n");
}

/* functie care afiseaza histograma imaginii */
void histogram_image (struct image *pic, int *x, int *y) {

   int i, j;
   int max_sum_freq = -1;
   int frequency[256] = {0}, sum_freq[256] = {0}, asterisk_num[256] = {0};
   // interval intre binuri pt suma
   int gap; 
   gap = 256 / *y;
   //daca imaginea e de tip pgm, nu se poate aplica comanda 
   if ( pic -> magic_num[1] == '3' || pic -> magic_num[1] == '6') {
      printf ("Black and white image needed\n");
   }
   else if (pic -> magic_num[1] == '2') {
      //contorizez frecventa fiecarui pixel
      for (i = 0; i < pic -> height; i++) {
         for (j = 0; j < pic -> width; j++) {
            frequency[pic -> pixel_ascii[i][j]] ++;
         }
      }
      for (i = 0; i < PIXEL_RANGE; i++) {
         sum_freq[i / gap] += frequency[i]; 
      }
      // aflu maximul dintre frecvente
      for (i = 0; i < PIXEL_RANGE; i++) {
         if (sum_freq [i / gap] > max_sum_freq) {
            max_sum_freq = sum_freq[i / gap];
         }
      }
      //aflu numarul de stelute 
      for (i = 0; i< *y; i++) {
         asterisk_num[i] = sum_freq[i] * (*x) / max_sum_freq ;
      }
      //afisez histograma
      for (i = 0; i < *y; i++) {
         printf ("%d", asterisk_num[i]);
         printf ("\t");
         printf ("|");
         printf ("\t");
         for (j = 0; j < asterisk_num[i]; j++) {
            printf ("*");
         }
         printf ("\n");

      }
   }
   else if (pic -> magic_num[1] == '5') {
      //contorizez frecventa fiecarui pixel
      for (i = 0; i < pic -> height; i++) {
         for (j = 0; j < pic -> width; j++) {
            frequency[(int)pic -> pixel_binary[i][j]] ++;
         }
      }
      for (i = 0; i < PIXEL_RANGE; i++) {
         sum_freq[i / gap] += frequency[i];
      }
      // aflu maximul dintre frecvente
      for (i = 0; i < PIXEL_RANGE; i++) {
         if (sum_freq[i / gap] > max_sum_freq) {
            max_sum_freq = sum_freq[i / gap];
         }
      }
      //aflu numarul de stelute 
      for (i = 0; i < *y; i++) {
         asterisk_num[i] = sum_freq[i] * (*x) / max_sum_freq ;
      }
      //afisez histograma
      for (i = 0; i < *y; i++) {
         printf ("%d", asterisk_num[i]);
         printf ("\t");
         printf ("|");
         printf ("\t");
         for (j = 0; j < asterisk_num[i]; j++) {
            printf ("*");
         }
         printf ("\n");
      }
   }
}

/* functie care face egalizarea unei imagini alb-negru */
void equalize_image (struct image *pic) {

   int new_value[256], freq_ap[256] = {0};
   int area = 0;
   int sum = 0;
   int i, j;
   //verific daca e imagine de tip ppm
   if (pic -> magic_num[1] == '3' || pic -> magic_num[1] == '6') {
      printf ("Black and white image needed\n");
   }
   else if (pic -> magic_num[1] == '2' || pic -> magic_num[1] == '5') {
      if (pic -> magic_num[1] == '2') {
         /* calculez suprafata imaginii*/
         area=pic->width*pic->height;
         //calculez frecevnta pixelilor
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               int value = pic -> pixel_ascii[i][j];
               freq_ap[value] ++;
            }
         }
         //calculez suma si aplic formula pentru ficare pixel
         for (i = 0; i <= 255; i++) {
            sum +=freq_ap[i];
            new_value[i] = (int)((double)255*sum/area);
         }
         //copiez noilea valori in matricea initiala
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               pic -> pixel_ascii[i][j] = new_value[pic -> pixel_ascii[i][j]];
            }
         }
      }
      else if (pic -> magic_num[1] == '5') {
         area=pic->width*pic->height;
         //calculez frecevnta pixelilor
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               int pixel=(int)pic->pixel_binary[i][j];
               freq_ap[pixel] ++;
            }
         }
         //calculez suma si aplic formula pentru ficare pixel
         for (i = 0; i <= 255; i++) {
            sum +=freq_ap[i];
            new_value[i] = (int)((double)255*sum/area);
         }
         //copiez noilea valori in matricea initiala, dar le transform in ascii
         for (i = 0; i < pic -> height; i++) {
            for (j = 0; j < pic -> width; j++) {
               int pixel=(int)pic->pixel_binary[i][j];
               pic -> pixel_binary[i][j] = new_value[pixel];
            }
         }
      }
      printf("Equalize done\n");
   }
}

/* functie care reduce imaginea la cea cuprinsa in cazul selectiei curente */
void crop_image (struct image *pic) {

   int rows, column, columns, i ,j;
   //calculez nr coloane din matricea noua
   column = pic -> x2 - pic -> x1;
   //coloane pentru matricile de tip P3 si P6
   columns = column * 3;
   //calculez nr linii din matricea noua
   rows = pic -> y2 - pic -> y1;
   if (pic -> magic_num[1] == '2') {
      //aloc memorie pentru o matrice auxiliara
      int **cropped;
      cropped = (int **)malloc(rows * sizeof(int *));
      if (cropped == NULL) {
         printf ("No memory allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         cropped[i] = (int *)malloc(column * sizeof(int));
         if (cropped[i] == NULL) {
            printf ("Memory not allocated.\n");
            exit(-1);
         }
      }
      //completez matricea auxiliara cu pixelii din matricea initiala 
      for (i = pic->y1; i < pic->y2; i++) {
         for (j = pic->x1; j < pic->x2; j++) {
            cropped[i-pic->y1][j-pic->x1] = pic ->pixel_ascii[i][j];
         }
      }
      //eliberez matricea veche
      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_ascii[i]);
      }
      free (pic -> pixel_ascii);
      //aloc din nou o matrice, dar cu noile valori pentru linii si coloane
      pic -> pixel_ascii = (int **)malloc(rows * sizeof(int *));
      if (pic -> pixel_ascii == NULL) {
         printf ("No memory allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         pic -> pixel_ascii[i] = (int *)malloc(column * sizeof(int));
         if (pic -> pixel_ascii[i] == NULL) {
            printf ("Memory not allocated.\n");
            exit(-1);
         }
      }
      //copiez valorile stocate in matricea auxiliara
      for (i = 0; i < rows; i++) {
         for (j = 0; j < column; j++) {
            pic -> pixel_ascii[i][j] = cropped[i][j];
         }
      }
      //eliberez matricea auxiliara
      for (i = 0; i < rows; i++) {
         free (cropped[i]);
      }
      free (cropped);
      //retin noile coordonate ale imaginii
      pic->width=column;
      pic->height=rows;
      pic->x1=0;
      pic->x2=column;
      pic->y1=0;
      pic->y2=rows;

   }
   else if (pic -> magic_num[1] == '3') {
      //aloc memorie pentru o matrice auxiliara
      int **cropped;
      cropped = (int **)malloc(rows * sizeof(int *));
      if (cropped == NULL) {
         printf ("No memory allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         cropped[i] = (int *)malloc(columns * sizeof(int));
         if (cropped[i] == NULL) {
            printf ("Memory not allocated.\n");
            exit(-1);
         }
      }
      //completez matricea auxiliara cu pixelii din matricea initiala
      for (i = pic->y1; i < pic->y2; i++) {
         for (j = pic->x1*3; j < pic->x2*3; j++) {
            cropped[i-pic->y1][j-pic->x1*3] = pic ->pixel_ascii[i][j];
         }
      }
      //eliberez matricea veche
      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_ascii[i]);
      }
      free (pic -> pixel_ascii);
      //aloc din nou o matrice, dar cu noile valori pentru linii si coloane
      pic -> pixel_ascii = (int **)malloc(rows * sizeof(int *));
      if (pic -> pixel_ascii == NULL) {
         printf ("No memory allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         pic -> pixel_ascii[i] = (int *)malloc(columns * sizeof(int));
         if (pic -> pixel_ascii[i] == NULL) {
            printf ("Memory not allocated.\n");
            exit(-1);
         }
      }
      //copiez valorile stocate in matricea auxiliara
      for (i = 0; i < rows; i++) {
         for (j = 0; j < columns; j++) {
            pic -> pixel_ascii[i][j] = cropped[i][j];
         }
      }
      //eliberez matricea auxiliara
      for (i = 0; i < rows; i++) {
         free (cropped[i]);
      }
      free (cropped);
      //retin noile coordonate ale imaginii
      pic->width=column;
      pic->height=rows;
      pic->x1=0;
      pic->x2=column;
      pic->y1=0;
      pic->y2=rows;
   }
   else if (pic -> magic_num[1] == '5') {
      //aloc memorie pentru o matrice auxiliara
      unsigned char **cropped;
      cropped = (unsigned char **)malloc(rows * sizeof(unsigned char *));
      if (cropped == NULL) {
         printf ("Memory not allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         cropped[i] = (unsigned char *)malloc(column * sizeof(unsigned char));
         if (cropped[i] == NULL) {
            printf ("No memory allocated.\n");
            exit(-1);
         }
      }
      //completez matricea auxiliara cu pixelii din matricea initiala
      for (i = pic->y1; i < pic->y2; i++) {
         for (j = pic->x1; j < pic->x2; j++) {
            cropped[i-pic->y1][j-pic->x1] = pic ->pixel_binary[i][j];
         }
      }
      //eliberez matricea veche
      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_binary[i]);
      }
      free (pic -> pixel_binary);
      //aloc din nou o matrice, dar cu noile valori pentru linii si coloane
      pic -> pixel_binary = (unsigned char **)malloc(rows * sizeof(unsigned char *));
      if (pic -> pixel_binary == NULL) {
         printf ("No memory allocated.\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         pic -> pixel_binary[i] = (unsigned char *)malloc(column * sizeof(unsigned char));
         if (pic -> pixel_binary[i] == NULL) {
            printf ("Memory not allocated.\n");
            exit(-1);
         }
      }
      //copiez valorile stocate in matricea auxiliara
      for (i = 0; i < rows; i++) {
         for (j = 0; j < column; j++) {
            pic -> pixel_binary[i][j] = cropped[i][j];
         }
      }
      //eliberez matricea auxiliara
      for (i = 0; i < rows; i++) {
         free (cropped[i]);
      }
      free (cropped);
      //retin noile coordonate ale imaginii
      pic->width=column;
      pic->height=rows;
      pic->x1=0;
      pic->x2=column;
      pic->y1=0;
      pic->y2=rows;
   }
   else if (pic -> magic_num[1] == '6') {
      //aloc memorie pentru o matrice auxiliara
      unsigned char **cropped;
      cropped = (unsigned char **)malloc(rows * sizeof(unsigned char *));
      if (cropped == NULL) {
         printf ("Memory not allocated\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         cropped[i] = (unsigned char *)malloc(columns * sizeof(unsigned char));
         if (cropped[i] == NULL) {
            printf ("No memory allocated\n");
            exit(-1);
         }
      }
      //completez matricea auxiliara cu pixelii din matricea initiala
      for (i = pic->y1; i < pic->y2; i++) {
         for (j = pic->x1*3; j < pic->x2*3; j++) {
            cropped[i-pic->y1][j-pic->x1*3] = pic ->pixel_binary[i][j];
         }
      }
      //eliberez matricea veche
      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_binary[i]);
      }
      free (pic -> pixel_binary);
      //aloc din nou o matrice, dar cu noile valori pentru linii si coloane
      pic -> pixel_binary = (unsigned char **)malloc(rows * sizeof(unsigned char *));
      if (pic -> pixel_binary == NULL) {
         printf ("No memory allocated\n");
         exit(-1);
      }
      for (i = 0; i < rows; i++) {
         pic -> pixel_binary[i] = (unsigned char *)malloc(columns * sizeof(unsigned char));
         if (pic -> pixel_binary[i] == NULL) {
            printf ("Memory not allocated\n");
            exit(-1);
         }
      }
      //copiez valorile stocate in matricea auxiliara
      for (i = 0; i < rows; i++) {
         for (j = 0; j < columns; j++) {
            pic -> pixel_binary[i][j] = cropped[i][j];
         }
      }
      //eliberez matricea auxiliara
      for (i = 0; i < rows; i++) {
         free (cropped[i]);
      }
      free (cropped);
      //retin noile coordonate ale imaginii
      pic->width=column;
      pic->height=rows;
      pic->x1=0;
      pic->x2=column;
      pic->y1=0;
      pic->y2=rows;
   }

}

/* functia cu care aplic un anumit efect pe o imagine de tip P3 sau P6 */
void apply_efect (struct image *pic, int ker[3][3], int coef) {

   int sum;
   int i, j, k, l;
   //initializez coordonatele pe care voi aplica efectele 
   int x_i = pic -> x1;
   int x_f = pic -> x2;
   int y_i = pic ->y1;
   int y_f = pic -> y2;
   if (pic -> magic_num[1] == '3') {
      //aloc o matrice noua in care sa retin efectul dupa comanda
      int **mat;
      mat = (int **)malloc(pic -> height * sizeof(int *));
      if (mat == NULL) {
         printf ("Memory not allocated\n");
         exit(-1);
      }
      
      for (i = 0; i < pic ->height; i++) {
         mat[i] = (int *)malloc(pic -> width * 3 * sizeof(int));
         if (mat[i] == NULL) {
            printf ("Memory not allocated\n");
            exit(-1);
         }
      }
      //primul pixel nu il iau in considerare
      if (x_i == 0) { 
         x_i = 1;
      }
      //primul pixel nu il iau in considerare
      if (y_i == 0) { 
         y_i = 1;
      }
      //ultimul pixel nu il iau in considerare
      if (x_f == pic -> width) { 
         x_f = pic -> width - 1;
      }
      //ultimul pixel nu il iau in considerare
      if (y_f == pic -> height) { 
         y_f = pic -> height - 1;
      }
      //for-uri pentru intreaga selectie 
      for (i = y_i; i < y_f; i++) {
         for (j = x_i * 3; j < x_f * 3; j++) {
            sum = 0;
            //for-uri pentru matricea aferenta fiecarui pixel
            for (k = -1; k < 2; k++) {
               for (l = -3; l < 4; l=l+3) {
                  sum = sum + pic -> pixel_ascii[k + i][l + j] * ker[k  + 1][l / 3 + 1];
               }
            }
            //pentru cazurile BLUR si GAUSSIAN_BLUR
            if (coef != 0) {
               mat[i][j] = sum / coef;
            }
            //in cazul lui EDGE si SHARPEN, coef este 0
            else {
               mat[i][j] = sum;
            }
            //limitarea valorilor in caz ca pixelii sunt negativi sau depasesc 255
            if (mat[i][j] < 0) {
               mat[i][j] = 0;
            }
            else if (mat[i][j] > 255) {
               mat[i][j] = 255;
            }
         }
      }
      //copiez in matricea initiala noile valori
      for (i = y_i; i < y_f; i++) {
         for (j = x_i * 3; j < x_f * 3; j++) {
            pic -> pixel_ascii[i][j] = mat[i][j];
         }
      }
      //eliberez matricea auxiliara
      for(int i=0;i<pic->height;i++)
         free(mat[i]);
      free(mat);
   }
   else if (pic -> magic_num[1] == '6') {
      //aloc o matrice noua in care sa retin efectul dupa comanda
      int **mat;
      mat = (int **)malloc(pic -> height * sizeof(int *));
      if (mat == NULL) {
         printf ("Memory not allocated\n");
         exit(-1);
      }
      for (i = 0; i < pic ->height; i++) {
         mat[i] = (int *)malloc(pic -> width * 3 * sizeof(int));
         if (mat[i] == NULL) {
            printf ("Memory not allocated\n");
            exit(-1);
         }
      }
      //primul pixel nu il iau in considerare
      if (x_i == 0) { 
         x_i = 1;
      }
      //primul pixel nu il iau in considerare
      if (y_i == 0) { 
         y_i = 1;
      }
      //ultimul pixel nu il iau in considerare
      if (x_f == pic -> width) { 
         x_f = pic -> width - 1;
      }
      //ultimul pixel nu il iau in considerare
      if (y_f == pic -> height) { 
         y_f = pic -> height - 1;
      }
      //for-uri pentru intreaga selectie 
      for (i = y_i; i < y_f; i++) {
         for (j = x_i * 3; j < x_f * 3; j++) {
            sum = 0;
            //for-uri pentru matricea aferenta fiecarui pixel
            for (k = -1; k < 2; k++) {
               for (l = -3; l < 4; l=l+3) {
                  sum = sum + pic -> pixel_binary[k + i][l + j] * ker[k  + 1][l / 3 + 1];
               }
            }
            //pentru cazurile BLUR si GAUSSIAN_BLUR
            if (coef != 0) {
               mat[i][j] = sum / coef;
            }
            //in cazul lui EDGE si SHARPEN, coef este 0
            else {
               mat[i][j] = sum;
            }
            //limitarea valorilor in caz ca pixelii sunt negativi sau depasesc 255
            if (mat[i][j] < 0) {
               mat[i][j] = 0;
            }
            else if (mat[i][j] > 255) {
               mat[i][j] = 255;
            }
         }
      }
      //copiez in matricea initiala noile valori
      for (i = y_i; i < y_f; i++) {
         for (j = x_i * 3; j < x_f * 3; j++) {
            pic -> pixel_binary[i][j] = mat[i][j];
         }
      }
      //eliberez matricea auxiliara
      for(int i=0;i<pic->height;i++)
         free(mat[i]);
      free(mat);
   }
}

/* functia care elibereaza toata memoria alocata */
void exit_program (FILE *file, struct image *pic) {

   int i,j;
   // dealocare pentru imaginile de tip pgm
   if (pic -> magic_num[1] == '2' || pic -> magic_num[1] == '3') {

      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_ascii[i]);
      }
      free (pic -> pixel_ascii);
   }
   //dealocare pentru imaginile de tip ppm
   else if (pic -> magic_num[1] == '5' || pic -> magic_num[1] == '6') {

      for (i = 0; i < pic -> height; i++) {
         free (pic -> pixel_binary[i]);
      }
      free (pic->pixel_binary);
   }
   //inchiderea fisierului
   fclose (file);
}


int main () {
	struct image pic;
   FILE *file,*file_out;
   char command[40];
   char filename[20];
   char *token;
   char in[30];
   int loaded=0;
   int i;
   
   do {
      scanf ("%s", command);
      
      if (strcmp (command, "LOAD") == 0) {
         if(loaded!=0){
            exit_program(file,&pic);
         }
         scanf ("%s", filename);  
         /* deschid fisierul pentru a citi din el */
         file = fopen(filename, "r");
         /* verific daca fisierul a fost deschis cu succes;
         daca nu a fost, opresc executia */
         if (file == NULL) {
            loaded=0;
            printf ("Failed to load %s\n", filename);
         } else {
            load_image(file, &pic);
            pic.x1=0;
            pic.y1=0;
            pic.y2=pic.height;
            pic.x2=pic.width;
            loaded ++;
            if (loaded != 0) {
               printf ("Loaded %s\n", filename);
            }
         }
      }
      else if (strcmp (command, "SAVE") == 0) {
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            fgets (in, 30, stdin);
            printf ("No image loaded\n");
         }
         else {
            fgets (in, 30, stdin);
            token = strtok (in, " \n");                       
            strcpy (filename, token);
            token = strtok (NULL, " \n");
            /*verific ce a ramas in token si apelez fie save_image_ascii, 
            daca am gasit parametrul, fie save_image*/
            if (token!=NULL) {
               save_image_ascii (file_out, &pic, filename);
               printf ("Saved %s\n", filename);
            }
            else {
               save_image(file_out, &pic, filename);
               printf ("Saved %s\n", filename);
            }
         } 
      }
      else if (strcmp (command, "SELECT") == 0) {
         int x1, x2, y1, y2;
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            char error[256];
            fgets(error,256,stdin);
            printf ("No image loaded\n");
         }
         else {
            //citesc intregul sir 
            fgets (in, 30, stdin);
            if (sscanf (in, "%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
               //am gasit cele 4 variabile si apelez functia de selectie partiala
               select_part_image (&pic, &x1, &y1, &x2, &y2);
            } else {
               /*altfel, verific daca este comanda invalida sau trebuie 
               sa apelez functia de selectie totala*/
               if(strcmp(in," ALL\n")==0 || strcmp(in," ALL \n")==0)
               select_all_image(&pic);
               else
               {
                  printf("Invalid command\n");
               }
               
            }
         }
      }
      else if (strcmp (command, "HISTOGRAM") == 0) {
         int x, y;
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            fgets (in, 30, stdin);
            printf ("No image loaded\n");
         }
         else {
            //citesc intregul sir 
            fgets (in, 30, stdin);
            if (strcmp(in, "\n") == 0) {
               printf("Invalid command\n");
            }
            //verific daca e imagine de tip ppm 
            else if (pic.magic_num[1] == '3' || pic.magic_num[1] == '6') {
               printf ("Black and white image needed\n");
            }
            else {
               //!= 2 <=> <2
               if (sscanf (in, "%d %d", &x, &y) != 2) { 
                  printf ("Invalid command\n");
               }
               else {
                  token = strtok (in, " ");
                  x = atoi(token);
                  token = strtok (NULL, " ");
                  y = atoi (token);
                  token = strtok (NULL, " ");
                  if (token != NULL) { 
                      //am mai gasit o variabila
                     printf ("Invalid command\n");
                  }
                  else {
                     //altfel, apelez functia 
                     histogram_image(&pic, &x, &y);
                  }
               }
            } 
         }
      }
      else if (strcmp (command, "EQUALIZE") == 0) {
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            printf ("No image loaded\n");
         }
         else {
            //apelez functia
            equalize_image (&pic);
         }
      }
      else if (strcmp (command, "CROP") == 0) {
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            printf ("No image loaded\n");
         }
         else {
            //apelez functia
            crop_image (&pic);
            printf ("Image cropped\n");
         }
      }
      else if (strcmp (command, "APPLY") == 0) {
         //nu a fost incarcat niciun fisier in memorie
         if (loaded == 0) {
            char error[256];
            fgets(error,256,stdin);
            printf ("No image loaded\n");
         }
         else {
            fgets(in, 30, stdin);
            if(strcmp(in,"\n")!=0){
            token = strtok(in, "\n");
            if(token[0]==' ')
               for(int i=0;i<strlen(token);i++)
                  token[i]=token[i+1];
            }
            if (token == NULL ||strcmp(in,"\n")==0) {
               printf("Invalid command\n");
            }
            
            else {
               //verific daca e incarcata o imagine de tip pgm
               if (pic.magic_num[1] == '2' || pic.magic_num[1] == '5') {
                  printf ("Easy, Charlie Chaplin\n");
               }
               else {
                  // edge
                  if (strcmp (in, "EDGE") == 0) {
                     int ker[3][3] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1} };
                     apply_efect(&pic, ker, 0);
                     printf ("APPLY EDGE done\n");
                  }
                  //sharpen
                  else if (strcmp (in, "SHARPEN") == 0) {
                     int ker[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };    
                     apply_efect(&pic, ker, 0);
                     printf ("APPLY SHARPEN done\n");
                  }
                  //blur
                  else if (strcmp (in, "BLUR") == 0) {
                     int ker[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
                     apply_efect(&pic, ker, 9);
                     printf ("APPLY BLUR done\n");
                  }
                  //gaussian blur
                  else if (strcmp (in, "GAUSSIAN_BLUR") == 0) {
                     int ker[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
                     apply_efect (&pic, ker, 16);
                     printf ("APPLY GAUSSIAN_BLUR done\n");
                  }
                  else {
                     printf ("APPLY parameter invalid\n");
                  }
               }
            }
         }
      }
      else
      {  // verific daca este o comanda invalida
         char error[256];
         fgets(error,256,stdin);
         if(strcmp(command,"\n")!=0 && strcmp(command," ")!=0 && strcmp(command,"EXIT")!=0)
         {
            printf("Invalid command\n");
         }
      }
      
   } while (strcmp(command, "EXIT") != 0);
   if(loaded!=0) 
      exit_program(file,&pic);
   else
      printf("No image loaded\n");
   return 0;
}