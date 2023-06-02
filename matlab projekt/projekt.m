%parametry modelu
dT = 1;
Tsym = 100;
p=0.1;
Hu = 20;
Hy = 25;
Ts = 0:dT:Tsym;
N = length(Ts);
n = 3;
m = 1;



%a=[1, -0.8832, 0]; % stara transmitancja
%b=[0, 0.005457];


a=[1, -0.7292, 0];
b=[5.762, 6.893];
%%% Assumes first parameter is zero so excluded



[CA,HA]=caha(conv(a,[1 -1]),1,Hy);
[Cb,Hb]=cbhb(b,1,Hy,Hu);
CAi=inv(CA);

H=CAi*Cb;
P2=CAi*Hb;
P1=-CAi*HA;


W = 2 * (H' * H + p*eye(Hu));
jedn = zeros([Hu 1]);
jedn(1,1) = 1;
pom = -(jedn' * inv(W));
%zmienne globalne
Yr = 42* ones([Hy 1]);
y = zeros(1, N+Hu);   %macierz wyjsc wszystkich
u =  zeros(1, N+Hu);  %macierz sterowania w calym zakresie
du = zeros(1, N+Hu);  %macierz zmian sterowania w calym zakresie
%zmienne wielkosci Hy zmieniajace sie z kazda petla
Yp = zeros([n 1]);    %macierz wyjsc poprzednich
dul = zeros([m 1]);    %dU poprzednich
Yk = zeros([Hy 1]); %macierz wyjsc kolejnych
duk = zeros([Hu 1]);    %dU przewidujace

Tp = 0:1:N-1+Hu;
DU0 = 0; %sygnal startowy 

start = 24;

for j = 1:N-1
    switch j
        case 1
            Yp = [start ;start ;start];
            dul = DU0;
        case 2
            Yp = [y(1) ;start ;start];
            dul = u(1)-DU0;
        case 3
            Yp = [y(2) ;y(1) ;start];
            dul = u(2)-u(1);
        otherwise
            Yp = [y(j-1) ;y(j-2) ;y(j-3)];
            dul = u(j-1)- u(j-2);
    end


    V = -2 * H' *(Yr - P1 * Yp - P2 * dul);
    du(j) = pom * V;
        
    

    if(j<2)
        for o = 1:1:Hu
            if(o==1)
                u(j+o-1) = du(j+o-1);
            else
                u(j+o) = u(j+o-1) + 0.01*du(j+o-1);    % kolejne du inaczej 
            end


            if(o == 1)
                duk(o) =  u(j+o);
            else
                duk(o) =  u(j+o)-u(j+o-1);
            end
        end
    else
        for o = 1:1:Hu
            u(j+o-1) = u(j+o-2) + du(j+o-2);
            duk(o) =  u(j+o)-u(j+o-1);
        end
    end

    Yk = H*duk + P1*Yp + P2 * dul;
    y(j) = Yk(1,1);
    subplot(2,1,1)
    plot(Tp(1,1:100),y(1,1:100),'o');
    subplot(2,1,2)
    plot(Tp(:,1:1:end-Hu),u(:,1:1:end-Hu),'-',Tp(:,1:1:end-Hu),du(:,1:1:end-Hu),'o')
    
end


