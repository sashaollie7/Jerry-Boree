#include <stdio.h>
#include "Jerry.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"

// a function which frees a string
status deleteKey(Element elem){
    if(!elem)
        return nullFail;
    free(elem);
    return success;
}

// a function which does nothing, it is needed to create a data structure
status printKey(Element elem){
    if (!elem)
        return nullFail;
    return success;
}

// a function which creates a deepcopy of a string
char* copyStr(char* str){
    if (!str)
        return NULL;
    char *temp =(char*)malloc((strlen(str)+1)*sizeof(char));
    strcpy(temp,str);
    return temp;
}

// a function which compares two strings returns true/false
bool cmpKey(Element this,Element that){
    if(!this||!that)
        return false;
    char* str1=(char*)this;
    char* str2=(char*)that;
    if(strcmp(str1,str2)==0)
        return true;
    return false;
}

// transform string into a number
// the number is the sum for each char's ASCII value and position in the string
int Transform(char* str){
    char* key = (char*) str;
    int i=0,sum=0,counter=1,length = strlen(key);
    while(i < length){
        sum+= counter*(unsigned char)key[i];
        counter++;
        i++;
    }
    return sum;
}

//a function which deletes a jerry and his characters fromm all the data structures
status removeJerry(GenList jerries,mvHash jerriesWithChar,hashTable jerriesByID,Jerry *jer){
    if(!jer||!jerries||!jerriesWithChar||!jerriesByID)
        return nullFail;
    for(int i=jer->num_of_char-1;i>=0;i--){ // delete each character from the chars hash table
        removeFromMultiValueHashTable(jerriesWithChar,jer->chars[i]->name,jer);
    }
    removeFromHashTable(jerriesByID,jer->ID);
    deleteNode(jerries,jer);
    return success;
}

//a function which receives a number and returns the prime number that is bigger and the closest to the given number
int findPrime(int num){
    if (num<2)
        return -1;
    int prime = num;
    if (num==2||num==3)
        return num;
    while(1){
        for (int i=2;i<=(prime/2);i++){ // if got to the half of the number then it is a prime
            if(prime%i==0)
                break;
            if(i==prime/2)
                return prime;
        }
        prime++;
    }
}


int main(int argc, char *argv[]) {
    //check if the program received enough arguments
   if (argc!=3)
        exit(1);
   // initiate variables for the menu
    int MAX_LENGTH =300,hap,num=0,numP = atoi(argv[1]);
    char buffer[MAX_LENGTH], *token,*line,sh='0',name[50], id[20], dmn[20],ch='0';
    double X, Y, Z;
    GenList ls,jerries,planets,chars;
    Jerry  *jer;
    Planet *pln;
    status stat;
    PhysicalCharacteristics *phC;
    ///initiate all the data structures for the program
    planets =  createLinkedList((CopyFunction) copyPlanet,(FreeFunction) deletePlanet,(PrintFunction) printPlanet,(EqualFunction) planetEqualKey);
    if(!planets){ // check if the Linked List was created
        printf("Memory Problem");
        exit(1);
    }
    jerries = createLinkedList((CopyFunction )shallowCopyJerry,(FreeFunction ) deleteJerry,(PrintFunction )printJerry,(EqualFunction )cmpJerry);
    if (!jerries) {
        destroyList(planets);
        printf("Memory Problem");
        exit(1);
    }// check if the Linked List was created
    chars = createLinkedList((CopyFunction )copyStr, deleteKey, printKey, cmpKey);
    if (!chars){
        destroyList(planets);
        destroyList(jerries);
        printf("Memory Problem");
        exit(1);
    }// check if the Linked List was created

    /// go through the file and retrieve all the data for the program
    FILE *fp = fopen(argv[2], "r"); // open the file
    if (fp == NULL) {
        printf("Error: could not open file %s", argv[2]);
        goto error;
    }
    fgets(buffer, MAX_LENGTH, fp); // this line always says "Planets"
    for (int i = 0; i < numP; i++) {

        /// retrieve all the data from the line into variables
        fgets(buffer, MAX_LENGTH, fp);
        token = strtok(buffer, ","); // this is the name of the planet
        strcpy(name, token);
        token = strtok(NULL, ",");
        X = strtod(token, NULL);
        token = strtok(NULL, ",");
        Y = strtod(token, NULL);
        token = strtok(NULL, ",");
        Z = strtod(token, NULL);

        ///initiate the planet and add it to the linked-list
        pln = initPlanet(name, X, Y, Z); //create the planet
        if (!pln)
            goto error;  // check if the planet created
        if(appendNode(planets,pln)!=success)
            goto error;
    }

    fgets(buffer, MAX_LENGTH, fp);// this line always says "Jerries"
    line = fgets(buffer, MAX_LENGTH, fp);

    while (line) {

        ///retrieve all the data from the line into variables
        token = strtok(buffer, ","); //this id the id
        strcpy(id, token);
        token = strtok(NULL, ","); // this is the dimension
        strcpy(dmn, token);
        token = strtok(NULL, ","); // this is the planet name
        strcpy(name, token);
        token = strtok(NULL, ","); // happiness level
        hap = atoi(token);

        pln = searchByKeyInList(planets,name); // search the right planet

        ///initiate the Jerry and add him to the data structures
        jer = initJerry(id, hap, dmn, pln);
        if (!jer)
            goto error;  // check if jerry was created
        if(appendNode(jerries,jer)!=success) // insert Jerry to the Jerries  list
            goto error;

        line = fgets(buffer, MAX_LENGTH, fp); //read the next line

        while (buffer[0] == '\t' || line) { // enter here only if the Jerry have characters in the file
            /// retrieve the data about the character
            token = strtok(buffer, "\t:"); // this is the name of the character
            strcpy(name, token);
            token = strtok(NULL, ":"); // this is the value of the character
            if (!token) // token is null only if reached the end of the file
                break;
            X = strtod(token, NULL);

            ///initiate the character and add it to jerry and the data structures
            phC = initPCharacter(name, X);
            if (!phC)
                goto error; // check if the character was created
            stat = addChar(jer, phC); // add the character to jerry
            if (stat != success)
                goto error; // check if added the character
            line = searchByKeyInList(chars,name);
            if (!line) {
                if (appendNode(chars, name) != success) // add the character to the characters list
                     goto error;
            }
            if(line){ //if found the same character delete the copy
                free(line);
                line=NULL;
            }
            line = fgets(buffer, MAX_LENGTH, fp); //read the next line
        }
    }
    fclose(fp);// close the file

    ///initiate all the hash tables for the program
    int numJ = getLengthList(jerries);
    int num4hash= findPrime(numJ);
    if (num4hash<5) // so the hash Table won't be so small
        num4hash=5;
    int numChars= findPrime(getLengthList(chars));
    if (numChars<5) // so the multi value hash Table won't be so small
        numChars=5;
    destroyList(chars);
    hashTable jerriesByID=createHashTable((CopyFunction )copyStr,deleteKey,printKey,(CopyFunction )shallowCopyJerry,(FreeFunction ) shallowDeleteJerry,
                                    (PrintFunction )printJerry,cmpKey,(TransformIntoNumberFunction )Transform,num4hash);
    if (!jerries){
        printf("Memory Problem");
        destroyList(jerries);
        destroyList(planets);
    }
    mvHash jerriesWithChar =createMultiValueHashTable((CopyFunction )copyStr,deleteKey,printKey,(CopyFunction )shallowCopyJerry,(FreeFunction )shallowDeleteJerry,(PrintFunction )printJerry,
                                                cmpKey, (EqualFunction )cmpJerry, (TransformIntoNumberFunction )Transform,numChars);
    if (!jerriesWithChar){
        printf("Memory Problem");
        destroyList(jerries);
        destroyList(planets);
        destroyHashTable(jerriesByID);
    }
    ///after all the hashtables were created insert the Jerries to them
    for (int i=1;i<=numJ;i++){
        jer = (Jerry*) getDataByIndex(jerries,i);
        if(addToHashTable(jerriesByID,jer->ID,jer)!=success)
            goto error;
        for(int j=0;j<jer->num_of_char;j++) { // insert the chars
            if (addToMultiValueHashTable(jerriesWithChar, jer->chars[j]->name, jer) != success)
                goto error;
        }
    }


    while (ch != '9') { //print the menu
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        scanf("%s", name);
        if (strlen(name) != 1) // if the user entered more than 1 char force the loop to go to default
            ch = 'a';
        else
            ch = name[0];
        switch (ch) {
            case '1':  /* adds a new Jerry to the daycare */

                //retrieve all the data to initiate the jerry from the user
                printf("What is your Jerry's ID ? \n");
                scanf("%s", name );
                jer = lookupInHashTable(jerriesByID,name); //search the jerry by ID in the hash table
                if (jer){
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                printf("What planet is your Jerry from ? \n");
                scanf("%s", dmn );
                pln = searchByKeyInList(planets,dmn); // search the planet from which is Jerry from
                if (!pln){ // if not found the planet is not in the system
                    printf("%s is not a known planet ! \n",dmn);
                    break;
                }
                printf("What is your Jerry's dimension ? \n");
                scanf("%s", dmn);
                printf("How happy is your Jerry now ? \n");
                scanf("%d", &num);

                //initiate the Jerry and add him to all the data structures
                jer = initJerry(name,num,dmn,pln);
                if (!jer){
                    goto error;
                }
                if (addToHashTable(jerriesByID,name,jer)!=success|| appendNode(jerries,jer)!=success)
                    goto error;

                printJerry(jer);
                break;

            case '2':  /* add physical characteristic to a Jerry */
                // retrieve all the data to add the character from the user
                printf("What is your Jerry's ID ? \n");
                scanf("%s", name );
                jer = lookupInHashTable(jerriesByID,name);
                if (!jer){
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic can you add to Jerry - %s ? \n",name);
                scanf("%s", name );
                if (jerryHasChar(jer,name)){
                    printf("The information about his %s already available to the daycare ! \n",name);
                    break;
                }
                printf("What is the value of his %s ? \n",name);
                scanf("%lf", &X );
                phC= NULL;

                // initiate the character and add it to the data structures
                phC = initPCharacter(name,X);
                if (!phC)
                    goto error;
                if (addChar(jer,phC)!=success){
                    deletePhysicalCharacteristics(phC);
                    goto error;
                }
                if (addToMultiValueHashTable(jerriesWithChar,name,jer)!=success)
                    goto error;

                // print all the Jerries with the given character
                printf("%s : \n",name);
                displayMultiValueHashTableElementsByKey(jerriesWithChar,name);
                break;
            case '3':  /* delete physical character from a Jerry */

                // retrieve all the data to delete a character from the user
                printf("What is your Jerry's ID ? \n");
                scanf("%s", name );
                jer = lookupInHashTable(jerriesByID,name);
                if (!jer){ // if there is no Jerry with the given character go here
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n",name);
                scanf("%s", dmn );
                if (!jerryHasChar(jer,dmn)){ // if Jerry doesn't have the character go here
                    printf("The information about his %s not available to the daycare ! \n",dmn);
                    break;
                }

                // delete the character from all the related data structures
                removeFromMultiValueHashTable(jerriesWithChar,dmn,jer);
                jerryDeleteChar(jer,dmn);
                printJerry(jer);
                break;
            case '4':  /* take a Jerry from the daycare */

                // retrieve the id and search for this Jerry
                printf("What is your Jerry's ID ? \n");
                scanf("%s", name );
                jer = lookupInHashTable(jerriesByID,name);
                if (!jer){ // if there is no Jerry with the given character go here
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                // remove the Jerry from all the data structures
                removeJerry(jerries,jerriesWithChar,jerriesByID,jer);
                jer=NULL;
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;
            case '5':  /* delete a similar Jerry */

                // retrieve the characters name
                printf("What do you remember about your Jerry ? \n");
                scanf("%s", name );
                ls = lookupInMultiValueHashTable(jerriesWithChar,name);
                if(!ls){ // if there are no Jerries with this char go here
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n",name);
                    break;
                }
                printf("What do you remember about the value of his %s ? \n",name);
                scanf("%lf", &X );
                Jerry * minJer;

                // search through the data structure for the most similar Jerry
                double temp,min=1000;
                for (int j=1;j<=getLengthList(ls);j++) { // compare all the Jerries that got this character
                    jer = getDataByIndex(ls,j);
                    for (int i = 0; i < jer->num_of_char; i++) {
                        if (strcmp(jer->chars[i]->name, name) == 0) {
                            temp = jer->chars[i]->number;
                            temp = temp - X;
                            if (temp < 0)
                                temp = -temp;
                            if (temp<min){
                                min = temp;
                                minJer=jer;
                            }
                        }
                    }
                }
                // print the Jerry and delete him
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(minJer);
                removeJerry(jerries,jerriesWithChar,jerriesByID,minJer); // remove the Jerry from all the data structures
                minJer=NULL;
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;
            case '6':  /* delete the saddest Jerry */
                num = getLengthList(jerries);
                if (!num){ // if there are no Jerries go here
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                min =101;

                //look for the Jerry with the lowest level of happiness
                for (int i=1;i<=num;i++){
                    jer = getDataByIndex(jerries,i);
                    if (jer->happiness<min){
                        min=jer->happiness;
                        minJer=jer;
                    }
                }

                // after checking all the Jerries print the saddest one and remove him
                printf("Rick this is the most suitable Jerry we found : \n");
                printJerry(minJer);
                removeJerry(jerries,jerriesWithChar,jerriesByID,minJer); // remove the Jerry from all the data structures
                minJer=NULL;
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;

            case '7':  /* Print stuff */
                // open sub menu
                printf("What information do you want to know ? \n");
                printf("1 : All Jerries \n");
                printf("2 : All Jerries by physical characteristics \n");
                printf("3 : All known planets \n");
                scanf("%s", name );
                if (strlen(name) != 1)
                    sh = 'a';
                else
                    sh = name[0];
                switch(sh){
                    case '1': // print all the jerries
                        if(!getLengthList(jerries)){ // first check if there are any Jerries
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        displayList(jerries);
                        break;

                    case '2': // print all the Jerries with a specific character
                        // retrieve the name of the character from the user
                        printf("What physical characteristics ? \n");
                        scanf("%s", name );
                        ls = lookupInMultiValueHashTable(jerriesWithChar,name); // retrieve the list of Jerries with the character
                        if(!ls){
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n",name);
                            break;
                        }

                        // print the list of Jerries with the character
                        printf("%s : \n",name);
                        displayList(ls);
                        break;
                    case '3': // print all the planets in the system
                        displayList(planets);
                        break;

                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '8':  /* add or subtract happiness */
                //first check if there are any Jerries in the system
                num =getLengthList(jerries);
                if(!num){
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                // if there are ,print the submenu
                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                scanf("%s", name );
                if (strlen(name) != 1) // if the user entered more than 1 char force the loop to go to default
                    sh = 'a';
                else
                    sh = name[0];
                switch(sh){
                    case '1': // Interact with fake Beth
                        for(int i=1;i<=num;i++){
                            jer = getDataByIndex(jerries,i);
                            hap = jer->happiness;
                            if (hap<20){ // decrease the happiness level by 5 if it was below 20
                                hap-=5;
                                if(hap<0)
                                    hap=0;
                            }
                            else { // raise the happiness level by 15 if it was above 19
                                hap+=15;
                                if(hap>100)
                                    hap=100;
                            }
                            jer->happiness=hap;
                        }

                        // when finished print all the Jerries
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        break;
                    case '2': // Play golf
                        for(int i=1;i<=num;i++){
                            jer = getDataByIndex(jerries,i);
                            hap = jer->happiness;
                            if (hap<50){ // decrease the happiness level by 10 if it was below 50
                                hap-=10;
                                if(hap<0)
                                    hap=0;
                            }
                            else { // raise the happiness level by 10 if it was above 49
                                hap+=10;
                                if(hap>100)
                                    hap=100;
                            }
                            jer->happiness=hap;
                        }

                        // when finished print all the Jerries
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        break;
                    case '3': // Adjust the picture settings on the TV
                        for(int i=1;i<=num;i++){ //raise the level of happiness by 20 to all the Jerries
                            jer = getDataByIndex(jerries,i);
                            jer->happiness=jer->happiness+20;
                            if(jer->happiness>100)
                                jer->happiness=100;
                        }

                        // when finished print all the Jerries
                        printf("The activity is now over ! \n");
                        displayList(jerries);
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;
            case '9':  /* delete all and close the daycare */
                destroyMultiValueHashTable(jerriesWithChar);
                destroyHashTable(jerriesByID);
                destroyList(jerries);
                destroyList(planets);
                printf("The daycare is now clean and close ! \n");
                return 0;
            default:
                printf("Rick this option is not known to the daycare ! \n");
                break;
        }

    }

    /// in case of an error delete all the data structure and exit
    error:
    printf("Memory Problem");
    destroyMultiValueHashTable(jerriesWithChar);
    destroyHashTable(jerriesByID);
    destroyList(jerries);
    destroyList(planets);
    exit(1);
}