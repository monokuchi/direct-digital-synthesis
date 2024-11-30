
from scipy import signal
import numpy as np
import matplotlib.pyplot as plt



# Define our constants
NUM_BITS = 16
NUM_POINTS = 2**NUM_BITS


def to_c_array(values, ctype="float", name="table", formatter=str, colcount=8):
    # apply formatting to each element
    values = [formatter(v) for v in values]

    # split into rows with up to `colcount` elements per row
    rows = [values[i:i+colcount] for i in range(0, len(values), colcount)]

    # separate elements with commas, separate rows with newlines
    body = ',\n    '.join([', '.join(r) for r in rows])

    # assemble components into the complete string
    return '{} {}[] = {{\n    {}}};'.format(ctype, name, body)



def luts_to_str(luts: dict) -> str:
    ret_str = ""
    for name, waveform in luts.items():
        ret_str += (f"--------------- {name} Waveform, {NUM_POINTS} Samples ---------------" + "\n")
        ret_str += (to_c_array(waveform, name=f"{name.lower()}_lut") + "\n\n")
    return ret_str
        




# Generate our waveforms
t = np.linspace(0, 1, NUM_POINTS)
f = 1

sine = np.sin(2*np.pi*f*t)
square = signal.square(2*np.pi*f*t)
sawtooth = signal.sawtooth(2*np.pi*f*t)
triangle = signal.sawtooth(2*np.pi*f*t, 0.5)


# Plot the waveforms
fig = plt.figure()
fig.suptitle(f"LUT Waveforms, Number of Samples: {NUM_POINTS}", fontsize=16)

ax1 = plt.subplot(4, 1, 1)
ax1.set_title("Sine")
plt.plot(t, sine, "--go")

ax2 = plt.subplot(4, 1, 2)
ax2.set_title("Square")
plt.plot(t, square, "--go")

ax3 = plt.subplot(4, 1, 3)
ax3.set_title("Sawtooth")
plt.plot(t, sawtooth, "--go")

ax4 = plt.subplot(4, 1, 4)
ax4.set_title("Triangle")
plt.plot(t, triangle, "--go")

plt.show()


# Write the LUTs to a file
luts = {
    "Sine" : sine,
    "Square" : square,
    "Sawtooth" : sawtooth,
    "Triangle" : triangle
}
output_str = luts_to_str(luts)

f = open("lut.txt", "w")
f.write(output_str)
f.close()
