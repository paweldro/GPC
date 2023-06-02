ts = 60
ko = 35
tp = 250
kp = 46.74
s = tf('s');  

sys = (kp*exp(-ko*s))/(1+s*tp)
sys_z = c2d(sys, ts)
sys_DSP = filt(sys_z.Numerator, sys_z.Denominator, ts)


