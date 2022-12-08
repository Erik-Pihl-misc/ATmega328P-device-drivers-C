/********************************************************************************
* word.h: Inneh�ller funktionalitet f�r att anv�nda ord (16-bitars tal) som
*         en enhet eller separata byte (tv� 8-bitars tal).
********************************************************************************/
#ifndef WORD_H_
#define WORD_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* segmented_word: Strukt f�r att erh�lla ett ord (ett 16-bitars tal) i form
*                 av tv� separata byte (tv� 8-bitars tal).
********************************************************************************/
struct segmented_word
{
   uint8_t low;  /* De �tta minst signifikanta bitarna. */
   uint8_t high; /* De �tta mest signifikanta bitarna. */
};

/********************************************************************************
* word: Union f�r att kunna erh�lla ett tal som ett ord (ett 16-bitars tal)
*       eller i form av tv� byte (tv� 8-bitars tal).
********************************************************************************/
union word
{
   struct segmented_word segmented; /* Det 16-bitars talet uppdelat i tv� 8-bitars tal. */
   uint16_t whole;                  /* Det 16-bitars talet som en enda helhet. */
};

/********************************************************************************
* word_init: Initierar nytt objekt som lagrar angivet 16-bitars tal, vilket 
*            m�jligg�r att detta kan anv�ndas som en 16-bitars enhet eller
*            som tv� separata 8-bitars tal.
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