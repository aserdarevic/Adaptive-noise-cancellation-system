[y,Fs] = audioread('speechnoise.wav');
signal = y(:,2);
noise = y(:,1);
signoise = signal+noise;
    dt = 1/Fs;
    t = 0:dt:(length(signal)*dt)-dt;
    L = length(signal); 
%     nfft2 = 2^nextpow2(L);
%     ff = fft(signal, nfft2);
%     fff = ff(1:nfft2/2);
%     plot(abs(fff));
%     xfft = Fs*(0:nfft2/2-1)/nfft2;
%     figure
%     plot(xfft,abs(fff/L));
%     title('Freq domain signal');
    
FFTSignal = fft(signal);
FFTNoise = fft(noise);
P2 = abs(FFTSignal/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
   
f = Fs*(0:(L/2))/L;
plot(t,signal); xlabel('Seconds'); ylabel('Amplitude');
title ('signal');
figure
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of signal(t)')
xlabel('f (Hz)')
ylabel('|signal(f)|')
   
F2 = abs(FFTNoise/L);
F1 = F2(1:L/2+1);
F1(2:end-1) = 2*F1(2:end-1);
figure
plot(psd(spectrum.periodogram,signal,'Fs',Fs,'NFFT',length(signal)));
figure
plot(t,noise); xlabel('Seconds'); ylabel('Amplitude of refnoise');  
title('refnoise');
figure 
plot(f,F1) 
title('Single-Sided Amplitude Spectrum of refnoise(t)')
xlabel('f (Hz)')
ylabel('|refnoise(f)|')
figure
plot(psd(spectrum.periodogram,noise,'Fs',Fs,'NFFT',length(noise)));


   
   
  
     
     
     
     