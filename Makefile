# Makefile
#
# Created on: 13.11.2009
#     Author: Andrea Palmat�
#
#

CC = ppc-amigaos-gcc

#DEBUG = -gstabs -DDEBUG
CFLAGS = -O3 -mstrict-align -ffast-math $(DEBUG) -I. -Iinclude -Wall -DCATCOMP_NUMBERS -DCATCOMP_STRINGS
LDFLAGS = -athread=native -use-dynld -Wl,-export-dynamic -Wl,--no-undefined
LIBS = -Llib -liodbc -liodbcinst -ldl -lpthread -lunix -lauto -lraauto 
EXENAME = iODBC

SRC = iODBC_glue.c iODBC_misc.c iODBC_NewDSN.c iODBC_NewDSN_Wizard.c iODBC_CreateDSN.c iODBC_ConfigDSN.c iODBC_connect.c iODBC_cat.c iODBC.c 
        
OBJ = $(SRC:.c=.o)

all: iODBC

iODBC: $(OBJ)
	$(CC) $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $(EXENAME) $(OBJ) $(LIBS)
	echo "Done"

clean:
	rm -f $(OBJ) $(EXENAME)
    
