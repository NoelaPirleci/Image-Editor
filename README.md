# Image-Editor
/Pirleci Noela-Elena, grupa 315CA

	Pentru a implementa editorul foto aferent temei 3, am decis 
sa folosesc cate o functie distincta pentru fiecare comanda ce 
ar putea fi introdusa de la tastatura. Asadar, voi incepe sa 
prezint functinalitatea programului image_editor.c.

	In functia "main" am declarat o variabila de tip struct 
("pic"), iar in structura am retinut antetul unei 
poze si matricea de pixeli corespunzatoare acesteia. De asemenea
am retinut aici si variabilele "x1", "x2", "y1", "y2", care imi
vor fi de folos la functiile ce urmeaza. Dupa ce am declarat 
variabilele de care am nevoie, cu ajutorul unei structuri 
repetitve de forma "do...while" am inceput sa citesc rand pe 
rand cate o comanda, introdusa de la tastatura. Cu ajutorul 
functiei "strcmp", verific ce comanda este introdusa.

	Daca este introdusa comanda "LOAD", atunci, mai intai,
verific daca exista deja sau nu o imagine incarcata in
memorie. Daca exista, o voi inlatura. Imediat dupa, citesc 
numele unui fisier, urmand sa il deschid si, bineinteles, sa
verific daca acesta s-a deschis in regula. Daca nu a reusit 
operatia de deschidere, atunci afisez un mesaj corespunzator
("Failed to load"), dar altfel, apelez functia "load_image".
In functie, incep prin a citi din fisier antetul pozei. Dupa 
aceea, in functie de magic number-ul fiecarei poze, aloc memorie 
in care sa stochez pixelii imaginii. Daca imaginea este color, 
maresc width-ul imaginii de 3 ori, pentru a stoca toate cele 3
canale de culoare. Dupa ce fac alocarea si citesc si pixelii 
aferenti fiecarei imagini, ma reintorc in functia "main", 
unde modific variabilele "x1", "x2", "y1", "y2", cresc 
variabila "loaded", in care retin daca exista o imagine deja 
incarcata in memorie si afisez un mesaj corespunzator 
("Loaded image").

	Daca este comanda "SAVE", verific, din nou, daca exista
o imagine deja incarcata in memorie. Daca nu afisez un mesaj
("No image loaded"), altfel, citesc cu "fgets" sirul de dupa 
comanda. Folosesc functia "strtok" pentru a trunchia elementele
care se afla dupa "SAVE". In urma operatiilor, daca in variabila
"token" retin "ascii", atunci apelez functia "save_image_ascii",
altfel, apelez functia "save_image", care salveaza orice poza
in format binar.
	In functia "save_image_ascii", deschid mai intai fisierul de
iesire. Verific daca a reusit operatia. Daca in memorie e 
incarcata o imagine de tip P2 sau P3, voi scrie direct in fisier
antetul functiei si pixelii acesteia. In schimb, daca este 
incarcata o imagine de tip P5 sau P6, va trebui sa schimb magic 
number-ul functiei si, de asemenea, sa stochez pixelii din 
matricea binara in format ascii.
	In functia "save_image", deschid la fel ca la functia 
trecuta fisierul de iesire si verific daca a reusit operatia.
In schimb, la aceasta functie, implementarea va fi usor diferita
. Daca am o imagine de tip P5 sau P6, fiind deja in format
binar, o voi scrie direct in fisier. Dar, daca am o imagine de 
tip P2 sau P3, va trebui mai intai sa aloc o noua matrice in 
care sa retin noii pixeli. De asemenea, voi modifica magic 
number-ul si voi copia in matricea ajutatoare valorea pixelilor 
ascii in format binar. Dupa aceea, scriu in fisier noul antet 
si noii pixeli, iar apoi eliberez memoria insusita matricei 
auxiliare. 
	Dupa ce ma reintorc in main, indiferent de salvarea in tip 
ascii sau binar, afisez un mesaj corespunzator ("Saved 
nume_fisier").

	Daca este comanda "SELECT", verific, ca de fiecare data daca
exista vreun fisier deja incarcat in memorie si daca nu este 
afisez un mesaj corespunzator. Initializez variabilele "x1", 
"x2", "y1", "y2" in care retin extremele imaginii. Daca este 
incarcata o imagine in memorie voi citi cu "fgets" sirul de dupa
comanda. Daca acesta are fix 4 termeni, voi apela functia 
"select_part_image", insa daca nu, voi apela functia 
"select_all_image".
	In functia "select_part_image", voi verifica fiecare
posibilitate de comparatie a lui x1 cu x2 si y1 cu y2. Dupa ce 
voi fi verificat acestea, voi verifica si daca acestea depasesc 
valorile imaginii, iar daca asta se intampla, atunci voi afisa
un mesaj corespunzator ("Invalid set of coordinates"). Altfel, 
daca cele 4 coorodonate respecta toate conditiile, le voi atribui
si voi afisa mesajul "Selected x1, y1, x2, y2". 
	In functia "select_all_image", voi atribui variabilelor 
"x_i", "y_i", "x_f", "y_f" dimensiunile imaginii incarcate in 
memorie, iar apoi voi afisa mesajul "Selected ALL".

	Daca este comanda "HISTOGRAM", verific din nou daca este 
deja un fisier incarcat in memorie. Daca este, citesc cu "fgets"
ce urmeaza dupa comanda. Daca dupa "HISTOGRAM" nu intalneste 
niciun caracter, voi afisa mesajul "Invalid command". Altfel, 
daca este incarcata o imagine color, voi afisa mesajul 
"Black and white image needed". Altfel, voi verifica daca sunt 
citite doua variabile dupa comanda. Daca sunt citite fix doua,
atunci voi apela functia "histogram_image". Daca sunt citite
mai mult de 2 variabile, lucru pe care il verific cu functia 
"strtok", voi afisa mesajul "Invalid command".
	In functia "histogram_image" voi initializa 3 vectori de 
frecventa, in care sa retin freceventa fiecarui pixel, numarul 
de stelute pentru fiecare bin si suma pentru pixelii din "gap"
(interval intre binuri si suma) in functie de aparitiile 
acestora. Daca este incarcata o imagine de tip P2, voi verifica
frecventa fiecarui pixel ascii al imaginii si o voi retine in 
"frequency[]". Apoi, voi calcula suma in functie de frecventa
fiecarui pixel. Apoi, voi calcula si numarul de stelute ce 
va fi afisat pe ecran, pe care il retin in vectorul 
"asterisk_num". Dupa ce termin toate aceste operatii, afisez 
histograma imaginii.
	Daca este incarcata o imagine P5 in memorie, pasii vor fi 
aceiasi, doar ca pixelii binari vor fi convertiti in ascii.

	Daca este comanda "EQUALIZE", voi verifica din nou daca 
este incarcata vreo imagine in memorie. Daca nu, afisez mesaj 
corespunzator, altfel apelez functia "equalize_image".
	In functia "equalize_image" initializez 2 vectori de 
frecventa in care voi retine valorile noi si frecventa 
aparitiilor fiecarui pixel. Daca este incarcata o imagine 
color, voi afisa mesajul "Black and white image needed". 
Altfel, daca imaginea este de tip P2, voi calcula suprafata 
imaginii. Apoi, voi calcula frecventa fiecarui pixel, pe care o
stochez in vectorul "freq_ap". Apoi, voi calcula noua valoare
a fiecarui pixel cu ajutorul formulei din documentul pdf. La 
final, voi copia in matricea de pixeli noile valori.
	Daca imaginea este de tip P5, voi parcurge aceiasi pasi, 
doar ca voi converti mai intai matricea binara in int pentru a 
putea calcula frecventa fiecarui pixel si, de asemenea, voi 
converti si valoarea noua in format binar. La final, indiferent
de tipul imaginii, voi afisa mesajul "Equalize done".

	Daca este comanda "CROP", voi verifica mai intai daca 
este incarcata vreo imagine in memorie. Daca nu, afisez un 
mesaj corespunzator, daca da, apelez functia "crop_image".
	In functia "crop_image" voi intializa variabilele "rows", 
"column" si "columns" in functie de coordonatele "x1", "y1", 
"x2", "y2" dupa care trebuie sa fac decuparea. Indiferent de 
tipul imaginii, va trebui sa aloc o noua matrice (copy) de tip 
int (P2, P3) sau unsigned char(P5, P6), in care voi retine 
imaginea decupata. Apoi, in noua matrice voi copia pixelii 
corespunzatori din vechea matrice. Dupa ce ii atrbui, voi 
elibera vechea matrice si o voi aloca din nou conform noilor 
dimensiuni. Apoi, voi copia pixelii din matricea ajutatoare 
in cea initializata anterior. Dupa aceea, voi elibera si 
matricea ajutatoare. Dupa toate aceste operatii, reatribui 
dimensiunilor imaginii noile dimesiuni dupa operatie.

	Daca este comanda "APPLY", voi verifica mai intai daca e
incarcata in memorie vreo imagine. Daca nu, afisez un mesaj.
Altfel, citesc cu "fgets" sirul de dupa comanda, iar cu 
ajutorul functiei "strtok" il trunchiez. Daca dupa comanda nu 
mai exista niciun caracter, atunci voi afisa mesajul "Invalid
command". Altfel, daca imaginea incarcata este de tip P2 sau 
P5, voi afisa mesajul "Easy, Charlie Chaplin". Altfel, 
inseamna ca exista si dupa comanda ceva. Daca se intalneste 
"EDGE", "SHARPEN", "BLUR" sau "GAUSSIAN_BLUR", pentru fiecare 
in parte voi initializa matricea kernel, iar apoi voi apela 
functia "apply_efect". 
	In functia "apply_efect" voi initializa mai intai 
dimensiunile imaginii pe care voi aplica efectul. Indiferent 
de tipul imaginii (P3 sau P6), voi initializa o matrice de tip
int (P3) sau unsigned char (P6), in care voi retine noua 
matrice. Initial, voi verifica daca incep de la pixelul 0, 
pentru a putea sari peste el sau daca merg pana la sfarsitul
imaginii, pentru a sti sa evit ultimul pixel. Apoi, cu 
ajutorul a 2 for-uri voi initializa de fiecare data suma cu 0. 
Dupa, cu ajutorul a doua alte for-uri voi parcurge matricea de 
3x3 ce inconjoara pixelul. Voi aplica pe fiecare pixel formula
specifica efectului cerut. Voi verifica, de asemenea, daca
valoarea pixelului depaseste valoarea 255 sau daca este 
negativ, pentru a il putea rotunji. Apoi, voi copia in vechea
matrice noile valori. La final, cand ma voi reintoarce in 
functia "main", voi afisa un mesaj corspunzator. De asemenea, 
daca parametrul introdus dupa comanda nu este unul valid, 
voi afisa mesajul "APPLY parameter inavlid".

	Structura "do...while" se va opri atunci cand va intalni 
comanda "EXIT". Daca exista o imagine incarcata in memorie, 
atunci va fi apelata functia "exit_program". Daca nu este 
incarcata nicio imagine, voi afisa un mesaj.
	In functia "exit_program" voi dealoca matricea folosita 
pentru a retine pixelii unei imagini si voi inchide fisierul.
	Apoi, programul se va termina. 
