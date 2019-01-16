

7-Segment Displays - Multiplexing by Segment
--------------------------------------------


Note:  There is a video associated with this repo:

https://www.youtube.com/watch?v=8w09Zy8MQrc


Multiplexing 7-segment displays by segment can eliminate
the need for resistors on the segment driver lines, and
transistors on the common anode/cathode pins, as required
for the traditional multiplexing by digit.  All of that is
replaced by individual resistors on each CA or CC pin.

Schematics are included here showing the traditional
circuit used for multiplexing by digit, and the alternate
of multiplexing by segment.  A circuit is also shown for
the alternate method (for CC displays only) using a
74HC4017 chip to drive the segments to minimize the number
of processor pins used for the displays.  The number of MCU
pins needed with the 4017 setup equals the number of
digits, plus one.

Also included are Arduino sketches for the Nano providing
demonstration code for both the regular by-segment method
and the 4017 method.  The sketches have more details on how
this all works.

One Segment at a Time
---------------------

In this method, one segment driver line is turned on at a
time, which drives that same segment on all digits.  The
state of the CA/CC lines determines on which digit(s) that
segment lights up.  A single resistor on the CA/CC line is
sufficient because those lines only source or sink currrent
for one segment at a time.  However, a segment driver line
may need to power its segment on all digits at the same
time.  That is a limiting factor for this method, but use
of high-efficiency low-current displays, such as the Vishay
TDSR 1360-IK, may mitigate that problem.

Since each segment is ON only 1/7 of the time, the refresh
rate will have to be speeded up so that each segment will
be refreshed 40-70 times per second to avoid flicker or
shimmer. The included sketches update the display every two
millis, which is 500 Hz, or 71 Hz for an individual segment.
