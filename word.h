/********************************************************************************
* word.h: Innehåller funktionalitet för att använda ord (16-bitars tal) som
*         en enhet eller separata byte (två 8-bitars tal).
********************************************************************************/
#ifndef WORD_H_
#define WORD_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* segmented_word: Strukt för att erhålla ett ord (ett 16-bitars tal) i form
*                 av två separata byte (två 8-bitars tal).
********************************************************************************/
struct segmented_word
{
   uint8_t low;  /* De åtta minst signifikanta bitarna. */
   uint8_t high; /* De åtta mest signifikanta bitarna. */
};

/********************************************************************************
* word: Union för att kunna erhålla ett tal som ett ord (ett 16-bitars tal)
*       eller i form av två byte (två 8-bitars tal).
********************************************************************************/
union word
{
   struct segmented_word segmented; /* Det 16-bitars talet uppdelat i två 8-bitars tal. */
   uint16_t whole;                  /* Det 16-bitars talet som en enda helhet. */
};

/********************************************************************************
* word_init: Initierar nytt objekt som lagrar angivet 16-bitars tal, vilket 
*            möjliggör att detta kan användas som en 16-bitars enhet eller
*            som två separata 8-bitars tal.
*
*            - self: Pekare till objekt som ska initieras.
*            - num : Det 16-bitars tal som ska lagras.
********************************************************************************/
static inline void word_init(union word* self, 
                             const uint16_t num)
{
   self->segmented.low = 0;
   self->segmented.high = 0;
   self->whole = num;
   return;
}

#endif /* WORD_H_ */