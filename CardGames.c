#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//bit 6 is colour
//bit 2~5 is values
//bit 0, 1 suit

//c v s

// mask for colour     0100 0000   0x40

// mask for values     0011 1100   0x3c

// mask for suit       0000 0011   0x03

// mask for pair       0000 1111   0xf

// mask for hightValue 1111 0000   0xf0

//ace of heart         0100 0000   0x40
//spade of 2           0000 0111   0x07


/* handy typedefs */
typedef unsigned char card;
typedef unsigned char pairs;

/* arrays for the names of things */

static char *suits[] = {"Hearts","Diamonds","Clubs","Spades"};

static char *values[]= {"Ace","Two","Three","Four","Five","Six",\
                        "Seven","Eight","Nine","Ten","Jack",\
                        "Queen","King"};

static char *colour[]= {"Black","Red"};


/* function prototypes */
void printcard(card c); /* Displays the value of a card*/

void printdeck(card deck[52]); /* prints an entire deck of cards*/

void filldeck(card deck[52]); /* Populates a deck of cards */

void shuffle(card deck[52]); /* Randomizes the order of cards */

int compareface(const void* c1,const void *c2);
/* compares the face value of 2 cards, suitable to pass to qsort
   as the fourth argument */

pairs findpairs(card *hand); /* finds any pairs in a hand */



int main()
{
  card deck[52],*deckp;
  card hands[5][5],handssorted[5][5];
  pairs numpairs[5],highest;
  int hand,cd;
  int winner=-1;
  int counting = 0;

  srand(time(NULL));       /* seed the random number generator */



  /*populate and shuffle the deck */
  printf("Print Deck\n");
  filldeck(deck);
  printdeck(deck);
  printf("\n Shuffling Deck\n");
  shuffle(deck);
  printdeck(deck);
  printf("\n" );

  for(cd=0;cd<5;cd++)
  {
     for(hand=0;hand<5;hand++)
     {
        /* deal the hands here */
        hands[cd][hand]=deck[counting];
        counting++;
     }
  }

  for(hand=0;hand<5;hand++)
  {
    int num;
    int num2;
    int val1,val2;
    int card1,card2;
    int saveCard;
     /* sort the hands here */
     /* bubble sort*/
    // for(num=0;num<4;num++){
    //   for(num2=0; num2<4;num2++){
    //     card1=hands[hand][num2];
    //     card2=hands[hand][num2+1];
    //     val1 = (card1&0x3c)>>2;
    //     val2 = (card2&0x3c)>>2;
    //     if(val1>val2){
    //       saveCard = hands[hand][num2];
    //       hands[hand][num2] = hands[hand][num2+1];
    //       hands[hand][num2+1] = saveCard;
    //     }
    //   }
    // }

    /* sort the hands*/

    qsort(hands[hand],5,sizeof(card), compareface);

    /*save the value into handssorted*/
    for(num=0;num<5;num++){
      card1=hands[hand][num];
      val1 = (card1&0x3c)>>2;
      handssorted[hand][num]=val1;
    }

  numpairs[hand]=findpairs(handssorted[hand]);

     printf("Hand %i:\n",hand+1);

     /* print the hands here */
     for(num=0;num<5;num++)
     {
        printcard(hands[hand][num]);
     }

     /* print the number and value of any pairs here */
     printf("Number of pairs: %d\n", numpairs[hand]&0x0f);
     if((numpairs[hand]&0x0f)!=0){
       printf("Highest pair is: %s\n\n", values[(numpairs[hand]&0xf0)>>4] );
     }
  }

  /* determine the winner and print it */
  int counter,hightCd,val1;

  for(counter=0;counter<5;counter++){
    hightCd=numpairs[counter];
    val1 = (hightCd&0xf0)>>4;
    if(val1>highest){
      winner=counter;
      highest=val1;
    }
  }
  /* winner -1 to determine where there is a winner or not*/
  if(winner == -1){
    printf("Drawn game\n \n");
  } else{
    printf("Winner is hand %d ",winner+1);
    printf("with a pair of %s\n \n", values[highest] );
  }
  return 0;
}

pairs findpairs(card *hand)
{
  pairs numpairs;
  int before,after;
  int count = 0;
  int hightValue=0x0;
  int p=0x0;
   /* find the pairs here */
   /* compares all value */
   for(before=0;before<5;before++){
     count--;
    for(after=0;after<5;after++){
      if(hand[before]==hand[after]){
        count++;
        if(count>1){
          hightValue=hand[before];
          p++;
          count=0;
        }
      }
    }
   }
   /*bug fix for 3,4 card with same value*/
   if(p==3){
     p=0x1;
   }
   if(p==4){
     p=0x2;
   }
   /*addup the bit*/
    hightValue = (hightValue)<<4;
    numpairs = hightValue + p;

   return numpairs;

}

void filldeck(card deck[52])
{
  int count = 0;
  int val = 0x0;
  int sui = 0x0;
  int col;
  int mixVal;
  int mixCol;
  int mixBinary;
  int i;
/*loop for a deck */
  for(i=0;i<52;i++){
/*count a~king,check for suit */
    if(count == 13){
      count = 0;
      val = 0x0;
      sui++;
    }
/*check colour for the suit */
    if(sui == 0x0 || sui == 0x1){
      col = 0x1;
    }else {
      col = 0x0;
    }

/*save the nuber into the deck array*/
    mixVal = val <<2;
    mixCol = col <<6;
    mixBinary = mixVal + sui + mixCol;
    deck[i] = mixBinary;

    val++;
    count++;
  }


   return;
}

void printdeck(card deck[52])
{
   int i;
   for(i=0;i<52;i++)
      printcard(deck[i]);
   return;
}

void printcard(card c)
{

  /* Using mask to find out the values, colour and suits */
  int su = (c&0x03);
  int va = (c&0x3c)>>2;
  int co = (c&0x40)>>6;

  // printf("%d\n", su );
  // printf("%d\n", va );
  // printf("%d\n", co );

  printf("\t %s of ", values[va]);
  printf("%s ,", suits[su]);
  printf("Is %s\n", colour[co]);
   /* print the value of the card here */
   return;
}

void shuffle(card deck[52])
{
   int i,rnd;
   int saveBinary;
   card c;

   for(i=0;i<52;i++)
   {
      /* generate a random number between 0 & 51 */
      rnd=rand() * 52.0 / RAND_MAX;
      /* finish shuffling the deck here */
      saveBinary = deck[i];
      deck[i]=deck[rnd];
      deck[rnd] = saveBinary;
   }

   return;
}


int compareface(const void* c1, const void *c2)
{
	/* This function extracts the two cards face values
	 and returns 1 if cd1 > cd2, 0 if cd1 == cd2, and
	 -1 otherwise. The weird argument types are for
	 compatibility with qsort(), the first two lines
	 decode the arguments back into "card".
	 */
	card cd1,cd2;

	cd1=*((card*) c1);
	cd2=*((card*) c2);

	cd1= (cd1&0x3c)>>2;
	cd2= (cd2&0x3c)>>2;

	if(cd1>cd2)
		return 1;
	if(cd1==cd2)
		return 0;

	return -1;
}
