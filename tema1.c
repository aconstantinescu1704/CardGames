#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct carte {
    char elem;
    char type;
    int val;
    struct carte* next;
} CarteJoc, *Tcarte;


typedef struct Joc {
    Tcarte pachet;
    int index;
    struct Joc *next;
    struct Joc *prev;
} Joc, *TJoc;

TJoc createJoc()
{
    TJoc new = malloc(sizeof(Joc));
    new->next = NULL;
    new->prev = NULL;
    return new;
}

TJoc InsertJoc(TJoc joc, Tcarte pachet, int index)
{
    TJoc last, new;

    new = createJoc();
    new->pachet = pachet;
    new->index = index;
    last = joc;

    if(joc == NULL)
    {
        joc = new;
        joc->next = joc;
        joc->prev = joc;
    }
    else
    {

        while(last->next != joc)
            last = last->next;

        last->next = new;
        new->prev = last;
        new->next = joc;
        joc-> prev = new;
    }


    return joc;

}

void freePachet(Tcarte pachet)
{
    if(pachet == NULL)
        return ;
    
    while(pachet != NULL)
    {   
        Tcarte t = pachet;
        pachet = pachet->next;
        free(t);
        t = NULL;
    }
}

TJoc DeleteJucator(TJoc joc)
{
    TJoc last, temp;
    last = joc;
    
    while(last->next != joc)
        last = last->next;

    last->next = joc->next;
    temp = joc;
    joc = joc->next;
    joc->prev = last;

    free(temp);
    temp = NULL;

    return joc;
}

void freeJoc(TJoc joc)
{
    
    if(joc == NULL)
        return ;
    

	//TODO: Cerinta 1g
    TJoc current = joc->next;

    if(joc->next == joc)
    {
        freePachet(joc->pachet);
        free(joc);
        joc = NULL;
    }
    else
    {
         while(current != joc)
        {
            TJoc t = current;
            current = current->next;
            freePachet(t->pachet);
            free(t);
            t = NULL;
        }

        freePachet(current->pachet);
        free(current);
        current = NULL;
    }

}

Tcarte createCarte()
{
    Tcarte pachet;
    pachet = malloc(sizeof(CarteJoc));
    pachet->next =  NULL;

    return pachet;
}

int min(int a, int b)
{
    if(a < b)
        return a;
    else
        return b;
}

char decodeNumber(int a)
{
    char cartiDeJocTip1[14] = "AKQJT98765432";
    
    for(int i = 0; i <= strlen(cartiDeJocTip1); i++)
    {
        if(i == 15 - a)
        return cartiDeJocTip1[i];
       
    }
}

Tcarte insertInceput(Tcarte pachet, char elem, char tip, int valoare)
{
    Tcarte carteInserat = createCarte();
    carteInserat->elem = elem;
    carteInserat->type = tip;
    carteInserat->val = valoare;

    carteInserat->next = pachet;
    pachet = carteInserat;

    return pachet;
}

Tcarte insertSfarsit(Tcarte pachet, char elem, char tip, int valoare)
{
    Tcarte carteInserat = createCarte();
    carteInserat->elem = elem;
    carteInserat->type = tip;
    carteInserat->val = valoare;

    Tcarte head = pachet;
    Tcarte current = pachet;
    Tcarte prev = pachet;

    if(pachet == NULL)
    {
        head = carteInserat;   
    }   
    else
    {
        current = head->next;
        prev = head;

        while(current != NULL)
        {
            current = current->next;
            prev = prev->next;
        }    
        
        if(current == NULL)
            prev->next = carteInserat;  
    }  
    
    return head;
}

int length(Tcarte pachet)
{
    Tcarte head = pachet;
    int lungime = 0;
    if(pachet == NULL)
        return 0;
    else
    {
        while(head != NULL)
        {
            lungime++;;
            head = head->next;
        }
    }

    return lungime;
}

Tcarte DeleteInceput(Tcarte pachet)
{
    Tcarte pachetNou = pachet;

    if(pachet == NULL || length(pachet) == 1)
    {
        free(pachet);
        return NULL;
    }
        
    else
    {
        Tcarte t = pachetNou;
        pachetNou = pachetNou->next;
        free(t);
        t = NULL;
        return pachetNou;
    }
}

void print(Tcarte pachet, FILE *output)
{
    Tcarte t = pachet;
    while(t != NULL)
    {
        fprintf(output, "%c%c ", t->elem, t->type);
        t = t->next;
    }
}

Tcarte inverse(Tcarte pachet)
{
    Tcarte prev, cur;

    prev = pachet;
    cur = pachet->next;
    pachet = pachet->next;

    prev->next = NULL; 

     while(pachet != NULL)
    {
        //retinem poz nodului viitor nodului curent
        pachet = pachet->next;
        //schimbam directia nodului current
        cur->next = prev;
        //inaintam prev si cur
        prev = cur;
        cur = pachet;
    }

        pachet = prev; 

    return pachet;
    
}

Tcarte Npiles(Tcarte pachet, int *indici, int nr)
{
    Tcarte pachetAmestecat = NULL;

    for(int i = 0 ; i < nr; i++)
    {
        Tcarte current = pachet;

        //mutam pointerul la inceputul fiecarui mini pachet indicat de indicele primit ca input
        for(int k = 0; k < (indici[i]-1) * (52 / nr); k++)
            current = current->next;

        // adaugam minipachetul la sfarsitul noului pachet 
        for(int j = 0; j < (52 / nr); j++)
        {
            pachetAmestecat = insertSfarsit(pachetAmestecat, current->elem, current->type, current->val);
            current = current->next;
        }

    }

    freePachet(pachet);

    return pachetAmestecat;

}

Tcarte intercalare(Tcarte pachet)
{
    Tcarte pachetAmestecat = NULL;
    Tcarte indexPachet1 = pachet;
    Tcarte indexPachet2 = pachet;

    //ducem pointerul la jumatatea pachetului
    for(int i = 0; i < 26; i++)
        indexPachet2 = indexPachet2->next;

    for(int i = 0; i < 26; i++)
    {
        pachetAmestecat = insertSfarsit(pachetAmestecat, indexPachet1->elem, indexPachet1->type, indexPachet1->val);
        pachetAmestecat = insertSfarsit(pachetAmestecat, indexPachet2->elem, indexPachet2->type, indexPachet2->val);

        indexPachet1 = indexPachet1->next;
        indexPachet2 = indexPachet2->next;

    }

    freePachet(pachet);

    return pachetAmestecat;
}

Tcarte mongean(Tcarte pachet)
{
    Tcarte pachetAmestecat = NULL;
    Tcarte head = pachet;

    while(head != NULL)
    {
        pachetAmestecat = insertSfarsit(pachetAmestecat, head->elem, head->type, head->val);
        head = head->next;

        if(head != NULL)
       {
        pachetAmestecat = insertInceput(pachetAmestecat, head->elem, head->type, head->val);
        head = head->next;
       }
    }

    freePachet(pachet);

    return pachetAmestecat;

}

void Razboi(Tcarte pachet, FILE* output)
{

    Tcarte jucator1 = NULL;
    Tcarte jucator2 = NULL;
    int i = 0;
    Tcarte current = pachet;

    int v1[52] = {0};
    int v2[52] = {0};

    //impartim pachetul in 2 pachete;
    for(int j = 0; j < 26; j++)
    {
        jucator1 = insertSfarsit(jucator1, current->elem, current->type, current->val);
        current = current->next;
    }

    for(int j = 0; j < 26; j++)
    {
        jucator2 = insertSfarsit(jucator2, current->elem, current->type, current->val);
        current = current->next;
    }

    // incepem sa iteram prin joc
    while(i < 100)
    {    
        if(jucator1 == NULL)
        {
            fprintf(output, "2");
            freePachet(jucator2);
            break;
        }    

        if(jucator2 == NULL)
        {
            fprintf(output, "1");
            freePachet(jucator1);
            break;
        }    

        if(jucator1->val > jucator2->val)
        {
            // mutam ambele carti de la inceputul pachetului jucatorilor la finalul listei castigatorului turei
            jucator1 = insertSfarsit(jucator1, jucator1->elem, jucator1->type, jucator1->val);
            jucator1 = DeleteInceput(jucator1);
            
            jucator1 = insertSfarsit(jucator1, jucator2->elem, jucator2->type, jucator2->val);
            jucator2 = DeleteInceput(jucator2); 

            if(jucator2 == NULL)
            {
                fprintf(output, "1");
                freePachet(jucator1);
                break;
            }

            i++;
        }

        if(jucator1->val < jucator2->val && jucator1 != NULL && jucator2 != NULL )
        {
            
            // mutam ambele carti de la inceputul pachetului jucatorilor la finalul listei castigatorului turei
            jucator2 = insertSfarsit(jucator2, jucator2->elem, jucator2->type, jucator2->val);
            jucator2 = DeleteInceput(jucator2);
            
            jucator2 = insertSfarsit(jucator2, jucator1->elem, jucator1->type, jucator1->val);
            jucator1 = DeleteInceput(jucator1);

            if(jucator1 == NULL)
            {
                fprintf(output, "2");
                freePachet(jucator2);
                break;
            }

            i++;
        }

        if(jucator1->val == jucator2->val && i < 100)
        {
            i++;
            int nr_razboi = min(min(length(jucator1) - 1, jucator1->val), min(length(jucator2) - 1, jucator2->val));

            Tcarte razboi1 = jucator1;
            Tcarte razboi2 = jucator2;
         
          //cautam cartile care vor decide castigatorul razboilui
            for(int j = 0; j < nr_razboi; j++)
            {
                razboi1 = razboi1->next;
                razboi2 = razboi2->next;
            }

            if(razboi1->val > razboi2->val)
            {
                razboi1 = NULL;
                razboi2 = NULL;

                //alcatuim pachetele de razboi ale fiecarui jucator
                for(int j = 0; j <= nr_razboi; j++)
                {
                    razboi1 = insertInceput(razboi1, jucator1->elem, jucator1->type, jucator1->val);
                    jucator1= DeleteInceput(jucator1);

                    razboi2 = insertInceput(razboi2, jucator2->elem, jucator2->type, jucator2->val);
                    jucator2 = DeleteInceput(jucator2);
                }

                //lipim cele 2 pachete obtinute din razboi
                while(razboi1 != NULL)
                {
                    jucator1 = insertSfarsit(jucator1, razboi1->elem, razboi1->type, razboi1->val);
                    razboi1 = DeleteInceput(razboi1);
                }

                while(razboi2 != NULL)
                {
                    jucator1 = insertSfarsit(jucator1, razboi2->elem, razboi2->type, razboi2->val);
                    razboi2 = DeleteInceput(razboi2);
                }

            }
            else
            {
                razboi1 = NULL;
                razboi2 = NULL;

                //alcatuim pachetele de razboi ale fiecarui jucator
                for(int j = 0; j <= nr_razboi; j++)
                {
                    razboi1 = insertInceput(razboi1, jucator1->elem, jucator1->type, jucator1->val);
                    jucator1= DeleteInceput(jucator1);

                    razboi2 = insertInceput(razboi2, jucator2->elem, jucator2->type, jucator2->val);
                    jucator2 = DeleteInceput(jucator2);
                }

                //lipim cele 2 pachete obtinute din razboi
                while(razboi2 != NULL)
                {
                    jucator2 = insertSfarsit(jucator2, razboi2->elem, razboi2->type, razboi2->val);
                    razboi2 = DeleteInceput(razboi2);
                }

                while(razboi1 != NULL)
                {
                    jucator2 = insertSfarsit(jucator2, razboi1->elem, razboi1->type, razboi1->val);
                    razboi1 = DeleteInceput(razboi1);
                }
            }
        }    
    }
    
    if(jucator1 != NULL && jucator2 != NULL)
    {        
        Tcarte copieJucator1 = jucator1;
        Tcarte copieJucator2 = jucator2;

        //construim vectorii de frecventa ai jucatorilor
       while(jucator1 != NULL)
        {
            v1[jucator1->val]++;
            jucator1 = jucator1->next;
        }

        while(jucator2 != NULL)
        {
            v2[jucator2->val]++;
            jucator2 = jucator2->next;
        }

        for(int k = 15; k >= 2; k--)
        {
            if(v1[k] < v2[k])
            {
                fprintf(output, "2 %c", decodeNumber(k));
                break;
            }  

            if(v1[k] > v2[k])
            {
                fprintf(output, "1 %c", decodeNumber(k));
                break;
            }
        }

        freePachet(copieJucator1);
        freePachet(copieJucator2);
    }

}

int getNchar(Tcarte pachet, char c)
{
    Tcarte carte = pachet;
    int nr = 0;

    while(carte != NULL)
    {
        if(carte->type == c)
            nr++;
        carte = carte->next;
    }

    return nr;
}

int WinnerFound(int mat[4][4])
{
    int ok;
    for(int i = 0; i < 4; i++)
    {  
        ok =  1;
          for(int j = 0; j < 4; j++)
        {
            if(mat[i][j] != 0)
                ok = 0;
    
        }    

        if(ok == 1)
            return i;

    }

    return -1;
}

int RollingStone(Tcarte pachet)
{
    Tcarte jucator[4];

     for(int i = 0; i < 4; i++)
        jucator[i] = NULL;


    Tcarte current = pachet;
    int matCarti[4][4];

    //construim pachetele fiecarui jucator pe baza pachetului initial 
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 13; j++)
        {
            jucator[i] = insertSfarsit(jucator[i], current->elem, current->type, current->val);
            current = current->next;
        }
    }
    
    //construim matricea de frecventa a tipurilor de carti
    for(int i = 0; i < 4; i++)
    {
        matCarti[i][0] = getNchar(jucator[i], 's');
        matCarti[i][1] = getNchar(jucator[i], 'h');
        matCarti[i][2] = getNchar(jucator[i], 'c');
        matCarti[i][3] = getNchar(jucator[i], 'd');
    }

    for(int i = 0; i < 4; i++)
    {
        freePachet(jucator[i]);
    }

    int startIndex = 0;
    int tip = 0;

    while(WinnerFound(matCarti) == -1)
    {
       int masa = 0;
    
        while(WinnerFound(matCarti) == -1)
        {
            matCarti[startIndex][tip]--;
            masa++;

            //iteram pe linii pana cand un jucator ramane fara carti dintr-un tip
            // le adaugam pe pozitia respectiva pe toate primite la tura respectiva
            if(matCarti[startIndex][tip] == -1)
            {
                matCarti[startIndex][tip] = masa - 1;
                break;
            }   

            //ne mutam pe urm linie, la urm jucator
            startIndex++;

            //asiguram circularitate
            if(startIndex == 4)
                startIndex = 0;           
        }

        //ne mutam pe urm coloana, la urm tip
        tip++;

        //asiguram circularitate
        if(tip == 4)
            tip = 0;
       
    }

    return WinnerFound(matCarti) + 1;
}

int GameoverScarabel(TJoc joc)
{
    TJoc nod = joc->next;

    int nr = 1;
    
    while( nod != joc)
    {
        nr++;
        nod = nod->next;
    }

    if(nr == 1)
        return 1;
    else
        return 0;
}

int isScarabel(char s)
{
    char scarabei[6] = "AKQJ";
    for(int i = 0; i < strlen(scarabei); i++)
    {
        if(s == scarabei[i])
            return 1;
    }
    return 0;
}

TJoc Scarabei(Tcarte pachet)
{
    Tcarte jucator[4];

    //punem pe null pachetele initiale de carti pt cei 4 jucatori
    for(int i = 0; i < 4; i++)
        jucator[i] = NULL;

    Tcarte current = pachet;
    
    //formam pachetele pt cei 4 jucatori
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 13; j++)
        {
            jucator[i] = insertSfarsit(jucator[i], current->elem, current->type, current->val);
            current = current->next;
        }

    TJoc joc = NULL;

    //construim jocul ca o lista dublu inlantuita circulara
    for(int i = 0; i < 4; i++)
        joc = InsertJoc(joc, jucator[i], i);

    //facem primul pas
    Tcarte tabla = NULL;
    tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
    joc->pachet = DeleteInceput(joc->pachet);

    int directie = 1;
    int scarabel = 0;
    int avansScarabel = 1;
 
    while(!GameoverScarabel(joc))
    {
        if(joc->pachet == NULL)
            joc = DeleteJucator(joc);
        
        // daca cartea de pe tabla de joc a fost scarabel 
        if( joc->pachet != NULL && isScarabel(tabla->elem) == 1)
        {
            scarabel = 1;
            avansScarabel = 1;

           //trec la urm carte  
           if(directie == 1)
               joc = joc->next;
            else
              joc = joc->prev;
            
            if(joc->pachet == NULL)
                joc = DeleteJucator(joc);

            //adaugam pe tabla urm cartea
            tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
   
            // verificam daca cartea urm dupa scarabel nu e 8
            if(joc->pachet != NULL && joc->pachet->elem == '8')
            {

                while(joc->pachet != NULL && joc->pachet->elem == '8')
                {
                    joc->pachet = DeleteInceput(joc->pachet);

                    if(directie == 1)
                        joc = joc->next; 
                    else
                        joc = joc->prev;

                    if(joc->pachet == NULL)
                        joc = DeleteJucator(joc);
                        
                    //adaugam pe tabla cartea cu urm
                    tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
                    avansScarabel++;
                }
                
                joc->pachet = DeleteInceput(joc->pachet);
            }
            else //va intra daca urm carte a fost alt scarabel
            {
                avansScarabel = 1;
                joc->pachet = DeleteInceput(joc->pachet);
            }
        }
        else
        {
            if(joc->pachet != NULL && tabla->elem == '8' && scarabel == 0)
            {
                
                // schimbam directia
                directie = -1 * directie;

                // trecem la urm jucator
                if(directie == 1)
                    joc = joc->next;  
                else
                    joc = joc->prev;
                
                // facem un pas si actualizam
                tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
                joc->pachet = DeleteInceput(joc->pachet); 

                if(joc->pachet == NULL)
                {
                    joc = DeleteJucator(joc);

                    if(directie == 1)
                        joc = joc->next;
                    else
                        joc = joc->prev; 
                }
         
            }

            if(joc->pachet != NULL && isScarabel(tabla->elem) == 0 && tabla->elem != '8' )
            {
                if(scarabel == 0)
                {
                    if(directie == 1)
                        joc = joc->next;  
                    else
                        joc = joc->prev;

                    if(joc->pachet == NULL)
                        joc = DeleteJucator(joc);
                    
                        
                    tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
                    joc->pachet = DeleteInceput(joc->pachet);

                    if(joc->pachet == NULL)
                    {
                        joc = DeleteJucator(joc);

                        if(directie == 1)
                            joc = joc->prev; 
                        else
                            joc = joc->next;
                    }

                }
                else
                {
                    scarabel = 0;
                    
                    while(avansScarabel != 0)
                    {
                        if(directie == 1)
                            joc = joc->prev;
                          
                        else
                            joc = joc->next;
                      
                        if(joc->pachet == NULL)
                            joc = DeleteJucator(joc);

                        //ne intoarcem pana la ultimul scarabel pus
                        avansScarabel--;
                    }

                    // cartile sunt luate de scarabelul de dinainte      
                    while(tabla != NULL)
                    {
                        joc->pachet = insertSfarsit(joc->pachet, tabla->elem, tabla->type, tabla->val);
                        tabla = DeleteInceput(tabla);
                    }

                    //actualizam tabla               
                    tabla = insertInceput(tabla, joc->pachet->elem, joc->pachet->type, joc->pachet->val);
                    joc->pachet = DeleteInceput(joc->pachet);

                }
            }
        }
    }

    freePachet(tabla);
  
    return joc;
}


int main()
{
    char cartiDeJocTip1[14] = "AKQJT98765432";
    char cartiDeJocTip2[5] = "shcd";
    int valoare[13] = {15, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2};

    Tcarte pachet = NULL;

    for(int i = 0; i < strlen(cartiDeJocTip1); i++)
    {

        for(int j = 0; j < strlen(cartiDeJocTip2); j++)
        {     
            pachet = insertSfarsit(pachet, cartiDeJocTip1[i], cartiDeJocTip2[j], valoare[i]);
        }
        
    }
    
    FILE *inputFile = fopen("in", "r");
    FILE *outputFile = fopen("out", "w");

    char miscare[30], buff[255];
    int N;
   
    fscanf(inputFile, "%s", miscare);
    fscanf(inputFile, "%d", &N);

    for(int i = 0; i < N; i++)
    {
        fscanf(inputFile, "%s", buff);

        if(strcmp(buff, "inversare") == 0)
        {
            pachet = inverse(pachet);
        }

        if(strcmp(buff, "npiles") == 0)
        {
            int n;
            fscanf(inputFile, "%d", &n);

            int *indiciNpiles = malloc(n * sizeof(int));

            fgets(buff, 40, inputFile);
           
            char *p = strtok(buff, ", ");
            int k = 0;
            while(p != NULL)
            {
                indiciNpiles[k] = atoi(p);
                p = strtok(NULL, " ");
                k++;
            }
            
            pachet = Npiles(pachet, indiciNpiles, n);

            free(indiciNpiles);

        }

        if(strcmp(buff, "intercalare") == 0)
            pachet = intercalare(pachet);

        if(strcmp(buff, "mongean") == 0)
            pachet = mongean(pachet);

    }

    if(strcmp(miscare, "amestecare") == 0)
    {
        print(pachet, outputFile);
       
    }

   if(strcmp(miscare, "razboi") == 0)
    {
        Razboi(pachet, outputFile);
    }

    if(strcmp(miscare, "rollingstone") == 0)
    {
        fprintf(outputFile, "%d\n", RollingStone(pachet));
    }

    if(strcmp(miscare, "scarabei") == 0)
    {
        TJoc out = Scarabei(pachet);
        fprintf(outputFile, "%d", out->index + 1);
         freeJoc(out);
    }
 
    freePachet(pachet);
    fclose(inputFile);
    fclose(outputFile);
}