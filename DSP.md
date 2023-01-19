# Delay
- Store the input signal into the first half of a buffer
- mix the second half of that buffer in with the input signal (ratio based on the dry/wet value)



# Designing Audio Effects Plug-Ins with C++

## [Filter Code Example](https://github.com/dimtass/DSP-Cpp-filters)

### Basic DSP Test Signals
You need to know the data sequences for several fundamental digital signals in order to begin understanding how the DSP theory works. The basic signal set consists of
• Direct Current (DC) and step: DC is a 0Hz signal
• Nyquist
• 1⁄2 Nyquist
• 1⁄4 Nyquist
• Impulse

### Conventions and rules:
• x(n) is always the input sequence; the variable n represents the location of the nth sample of the x- sequence.
• y(n) is always the output sequence; the variable n represents the location of the nth sample of the y- sequence.
• h(n) is the impulse response of the algorithm; a special sequence that represents the algorithm output for a single sample input or impulse.
• For real-time processing, the algorithm must accept a new input sample (or set of samples), do the processing, then have the output sample(s) available before the next input arrives; if the processing takes too long, clicks, pops, glitches, and noise will be the real-time result.

In algorithm diagrams, a delay is represented by a box with the letter z inside. The z-term will have an exponent
such as z−5 or z+2 or z0—the exponent codes the delay in samples following the same bookkeeping rules, with negative (−) exponents representing a delay in time backward (past samples) and positive (+) representing delay in forward time (future samples). You call z the delay operator and as it turns out, time-delay will be treated as a mathematical operation.

# Terms:

Orthogonal: is the engineering term for 90 degrees apart.
Euler's equation: ejωt = cos(ωt) + jsin(ωt)