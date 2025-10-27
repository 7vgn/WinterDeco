/**
 * @file programs.h
 * @date 2023-12-20
 * @brief Programs for creating different light patterns
 */

#ifndef PROGRAMS_H
#define	PROGRAMS_H

typedef void (*ProgramFunction)(uint16_t);

typedef struct
{
    char name[32];
    ProgramFunction initFunction, updateFunction;
} Program;

extern const Program PROGRAMS[];
extern const uint8_t NUMBER_OF_PROGRAMS;

#endif	/* PROGRAMS_H */
