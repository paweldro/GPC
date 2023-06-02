%%%parametry Hu, Hy
Hu = 32;
Hy = 32;

%%%transmitancja
a=[1, - 0.7866, 0];
b=[4.448, 5.525];
%%%liczenie macierzy
[CA,HA]=caha(conv(a,[1 -1]),1,Hy);
[CB,HB]=cbhb(b,1,Hy,Hu);

I = eye(Hu);
jed = zeros([1 Hu]);
jed(1,1) = 1;

%%%zapisanie macierzy do txt 
writematrix(CA);
writematrix(HA);
writematrix(CB);
writematrix(HB);
writematrix(I);
writematrix(jed);
%%%obliczenia do sprawdzenia wyniku

CAi=inv(CA);
H=CAi*CB;
P2=CAi*HB;
P1=-CAi*HA;

p = 0.5;
W = 2 * (H' * H + p*eye(Hu));