/*
 * DDS.h
 *
 *  Created on: Jan 7, 2025
 *      Author: john
 */

#ifndef INC_DDS_H_
#define INC_DDS_H_





/*
 * INCLUDES
 */
#include <stddef.h>


/*
 * DEFINES
 */
#define DDS_MAX_CUSTOM_WAVEFORMS 6
#define DDS_NUM_TRUNCATIONS      12


/*
 * ENUMS
 */
typedef enum
{
	SINE,
	SQUARE,
	SAWTOOTH,
	TRIANGLE,
	USER1,
	USER2,
	USER3,
	USER4,
	USER5,
	USER6
} Waveform;


/*
 * STRUCTS
 */
typedef struct
{
	/*
	 * Enum to indicate which waveform we are generating
	 */
	Waveform waveform;
} DDS;








/*
 * FUNCTIONS
 */
void DDS_Init(DDS *dds);

void DDS_Update(DDS *dds);








#endif /* INC_DDS_H_ */
