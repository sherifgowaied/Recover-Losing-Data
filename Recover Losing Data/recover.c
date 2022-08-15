#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;
const int size = 512 ;
int main(int argc, char *argv[])
{
    // make sure usser import 1 command line argument
    if (argc != 2)
    {
        printf("usage : ./recover filename");
        return 1 ;
    }
    FILE *file = fopen(argv[1], "r") ;

    if (file == NULL)
    {
        printf("couldn't open %s", argv[1]);
        return 1;
    }
    // decarling
    FILE *destination = NULL ;
    char jpg [8] ;
    BYTE buffer [512] ;
    int i = 0 ;
    while (fread(buffer, sizeof(BYTE), 512, file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // create the frist file by opening it and write 512 byte size of byte each time
            // i == 0 then 000.jpg
            if (i == 0)
            {
                sprintf(jpg, "%03i.jpg", i) ;
                destination = fopen(jpg, "w") ;
                fwrite(&buffer, sizeof(BYTE), 512, destination) ;
                i++ ;
            
            }
            //closing the frist file destiantion then opening new one (001.jgp) and adding 1 to i then i == 1 and so on
            else if (i > 0)
            {
                fclose(destination) ;
                sprintf(jpg, "%03i.jpg", i) ;
                destination = fopen(jpg, "w") ;
                fwrite(&buffer, sizeof(BYTE), 512, destination) ;
                i++ ;
            
            }
        
        }
        // if not start of new destination
        // then it keeps on writing the destination.
        else if (i > 0)
        {
            fwrite(&buffer, sizeof(BYTE), 512, destination);
        }
    }
    // closing 2 files 
    fclose(file) ;
    fclose(destination) ;
}