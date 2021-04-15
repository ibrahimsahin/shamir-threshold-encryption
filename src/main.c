#include <stdlib.h>
#include <stdio.h>

/*	polinomlarýn her bir terimi olarak kullanýlacak yapý */
typedef struct pol_oge *pol_gosterge;
typedef struct pol_oge{
	int katsayi;
	int us;
	pol_gosterge bag;
}pol_oge;


void encryption(int key,char * data,char *cipher_text)
{
    FILE *ptr;
    ptr = fopen (cipher_text,"w");
    fprintf(ptr,"Key=%d\n",key);
    int size=strlen(data),i;
    for(i=0;i<size;i++)
        fprintf(ptr,"%c",data[i]^key);
}

double key_calc(int x,double id[],double key[],int size)
{
    int i,j;
    double aratoplam=1,toplam=0;
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            if(i==j)
                continue;
            aratoplam=aratoplam*((x-id[j]) / (id[i]-id[j]));
        }
        toplam=toplam+(aratoplam*key[i]);
        aratoplam=1;
    }
    return toplam;
}

/*	Listede parametresinden sonra gelen öðeyi listeden ve bellekten siler */
void SonrakiTerimiSil( pol_gosterge Onceki ){
	pol_gosterge Silinecek = Onceki->bag; // free yapabilmek için adresi tutmalýyýz
	if( !Silinecek ) return;			  // zaten öðe yoksa iþlem yapýlamaz
	Onceki->bag = Onceki->bag->bag;		  // bað alanlarý düzgünce güncellenmelidir
	free(Silinecek);					  // bellekteki kalýntý temizlenmelidir
}

/*	Dinamik bellek alýp belirtilen üst ve katsayi bilgilerine sahip bir terim oluþturur
	ve Onceki parametresi ile verilen öðenin peþine liste yapýsýný bozmadan ekler */
pol_gosterge YeniTerimEkle( int Ust, int Katsayi, pol_gosterge Onceki ){
	pol_gosterge p = (pol_gosterge) malloc( sizeof(pol_oge) );
	if( !p ){ printf("Bellek yetmiyor yetmiyoor..."); exit(1); }
	p->us      = Ust;				// üst     bilgisini yaz
	p->katsayi = Katsayi;			// katsayý bilgisini yaz
	if( Onceki ){					// Onceki parametresi belirtildi
		p     ->bag = Onceki->bag;	// araya yada sona ekle
		Onceki->bag = p;
	} else p->bag = NULL;			// Onceki boþtur, baþka bir terimin peþine eklenmeyecek
	return p;
}

/*	Paralel olarak düzenlenmiþ þekilde verilen üsler ve katsayýlar dizilerindeki
	deðerlerle bir polinom listesi oluþturup liste baþýna gösterge döndürür */
pol_gosterge ListeDoldur( int *Carpanlar, int *Kuvvetler ){
	pol_gosterge ListeBasi=NULL, p = NULL;
	for( ; *Kuvvetler >= 0 && Carpanlar && Kuvvetler ; Carpanlar++, Kuvvetler++ ){
		p = YeniTerimEkle( *Kuvvetler, *Carpanlar, p );
		if( !ListeBasi ) ListeBasi = p; // ilk terimi ekleyince ListeBasi'ni ayarla
	}
	return ListeBasi;
}

/*	Liste olarak verilen iki polinomu birbiriyle çarpýp yeni bir polinom listesi oluþturur
	ve bu yeni polinomun liste baþý adresini döndürür.
	Polinomlardan en az bir tanesi sýfýr polinomu ise NULL döndürür.

*/
pol_gosterge PolCarp( pol_gosterge pBas, pol_gosterge qBas ){
	pol_gosterge rBas = NULL,		// yeni oluþturulacak sonuç listesi
				 p, q, r,			// pBas, qBas ve rBas (sonuç) listelerinde dolaþacak göstergeler
				 baslangic = NULL;	/* yeni oluþturulan bir elemanýn sonuç listesi üzerinde
									 ekleneceði konumun aranmaya baþlanacaðý yere gösterge */
	int rUst, rKat;					// her iki elemanýn çarpýmýnda üst ve katsayý bilgileri
	for( p = pBas ; p ; p = p->bag ){
		r = baslangic;				// q listesine her baþlayýþta r'yi son iþaretlenen (baslangic) konumdan baþlat
		for( q = qBas ; q ; q = q->bag ){
			rUst = p->us      + q->us;		// üstler toplanýr
			rKat = p->katsayi * q->katsayi;	// katsayýlar çarpýlýr
			if ( rBas ){					// listenin ilk elemaný deðil
				// uygun konumu ara
				while  (   r->bag && ( r->bag->us > rUst ) ) r = r->bag;
				// üssü bu olan eleman daha önce eklenmedi, yeni elemaný ekle
				if     (  !r->bag || ( r->bag->us < rUst ) ) YeniTerimEkle( rUst, rKat, r );
				// üssü ayný olan eleman zaten eklenmiþti, katsayýyý güncelle, katsayý sýfýrlanýrsa elemaný sil
				else if( ( r->bag->katsayi += rKat ) == 0  ) SonrakiTerimiSil(r);
			}
			else rBas = r = baslangic = YeniTerimEkle( rUst, rKat, NULL );	// listenin ilk elemaný eklendi
			if( q == qBas ) baslangic = r;	// p listesinin sonraki elemaný ile çarpýmlar bu konumdan itibaren yerleþecek
		}
	}
	return rBas;// sonuç listesinin baþýný döndür
}

double alt_hesapla(int x,double id[],double key[],int size)
{
    int i,j;
    double toplam=1;
        for(j=0;j<size;j++)
        {
            if(id[j]==x)
                continue;
            toplam=toplam*(x-id[j]);
        }
    return toplam;
}

void sub_polynomials(double id[],double key[],int size,char *cipher_text)
{

    FILE *ptr;
    ptr = fopen (cipher_text,"w");
    int i=0,j=0;
    pol_gosterge ListeBasi=NULL, p = NULL ,o=NULL;

    for(j=0;j<size;j++)
    {
        int sub=id[j],control=0;
        for(i=0;i<size;i++)
        {
            if(id[i]==sub)
                continue;
            o = YeniTerimEkle( 1, 1 , o );
            if( !ListeBasi ) ListeBasi = o; // ilk terimi ekleyince ListeBasi'ni ayarla
            o = YeniTerimEkle( 0, -id[i], o );
            if( !ListeBasi ) ListeBasi = o; // ilk terimi ekleyince ListeBasi'ni ayarla
            pol_gosterge list1 = ListeBasi;

            if(control==0)
            {
                p=list1;
                control++;
                ListeBasi=NULL; o=NULL;
                continue;
            }
            p=PolCarp(list1,p);
            ListeBasi=NULL; o=NULL;
        }
        double alt=alt_hesapla(id[j],id,key,size);
        fprintf(ptr,"%0.f : ",id[j]);
        for( ; p ; p = p->bag )
        {
            if(p->katsayi/alt<0)
                fprintf(ptr,"%.2f",p->katsayi/alt);
            else if(p->katsayi/alt>0)
                fprintf(ptr,"+%.2f",p->katsayi/alt);
            if(p->us==1)
                fprintf(ptr,"x");
            else if(p->us!=0)
                fprintf(ptr,"x^%d",p->us);
        }
        fprintf(ptr,"\n");
    }

}
void load(char *keys_file ,char *plain_text ,char *cipher_text)
{
    int size=0,i=0,j=0;
    char temp;
    char line[250];
    FILE *ptr;
    ptr = fopen (keys_file,"r");
    while (!feof(ptr))
    {
        fgets (line,250,ptr);
        size++;
    }
    fclose (ptr);

    double id[size];
    double key[size];
    ptr = fopen (keys_file,"r");
    while (!feof(ptr))
    {
        fscanf (ptr, "%lf", &id[i]);
        i++;
        fscanf(ptr, "%c", &temp);
        fscanf(ptr, "%c", &temp);
        fscanf(ptr, "%c", &temp);
        fscanf (ptr, "%lf", &key[j]);
        j++;
    }
    fclose (ptr);

    if(plain_text!=NULL)
    {
        int size_plain=0;
        ptr = fopen (plain_text,"r");
        while (!feof(ptr))
        {
            fgets (line,50,ptr);
            size_plain++;
        }
        fclose (ptr);

        char * data=(char *) malloc(size_plain*50);
        ptr = fopen (plain_text,"r");
        strcpy(data,"");
        while (!feof(ptr))
        {
            fgets (line,50,ptr);
            strcat(data,line);
        }
        encryption(key_calc(0,id,key,size),data,cipher_text);
    }
    else
        sub_polynomials(id,key,size,cipher_text);

}

void arrange(char **argc)
{
	if(strcmp(argc[1],"-e")==0)
		load(argc[2],argc[3],argc[4]);
	else if(strcmp(argc[1],"-l")==0)
		load(argc[2],NULL,argc[3]);
}

int main(int arg,char **argc)
{
    arrange(argc);
    return 0;
}
