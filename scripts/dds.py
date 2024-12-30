

from scipy import signal
import numpy as np
import matplotlib.pyplot as plt
import math



# Constants
BIT_DEPTH = 12
CLOCK_FREQ = 100e6 # 100 MHz system clock
F_RES_HZ = CLOCK_FREQ / 2**BIT_DEPTH
TUNING_WORD = 12
NUM_BIT_TRUNCATION = 4 # The number of bits we truncate from each integer in the phase accumulator starting from the LSB





def freq_to_tuning(freq):
    return freq * (2**BIT_DEPTH) / CLOCK_FREQ

def tuning_to_freq(tuning_word):
    return tuning_word * CLOCK_FREQ / (2**BIT_DEPTH)

def generate_sin_lut(length):
    amplitude = 2**(BIT_DEPTH-NUM_BIT_TRUNCATION-1) # The -1 is because the MSB of the word is the sign bit
    lut = [int(amplitude * math.sin(x/length * 2 * math.pi)) for x in range(length)] # Maps out 1 full period of a sin wave with full scale amplitude
    return lut




phase = np.zeros(1) # Array of integer phases used to index into our lookup tables (LUTs)
while (len(phase) < 2**BIT_DEPTH):
    phase += (phase[-1] + TUNING_WORD)


# plt.plot(phase)
# plt.title("DDS Phase Accumulator")
# plt.show()



# Truncate the integers inside our phase accumulator by NUM_BIT_TRUNCATION bits
# This is essentially quantizing the phase accumulator
for i in range(len(phase)):
    phase[i] = phase[i] >> NUM_BIT_TRUNCATION


plt.plot(phase)
plt.title("DDS Phase Accumulator (Truncated)")
plt.show()



sin_lut = generate_sin_lut(2**(BIT_DEPTH-NUM_BIT_TRUNCATION))

# plt.plot(sin_lut)
# plt.title("Sin LUT")
# plt.show()

print(len(phase))
print(len(sin_lut))

output_signal = [sin_lut[x] for x in phase]


plt.plot(output_signal)
plt.title("Output Signal")
plt.show()








