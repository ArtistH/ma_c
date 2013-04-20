/* spell.h */
#ifndef SPELL_H
#define SPELL_H

/* Define the maxinum size of words in the dictionary. */
#define SPELL_SIZE 31

/* public interface */
int spell(char (*dictionary)[SPELL_SIZE], int size, const char *word);

#endif
