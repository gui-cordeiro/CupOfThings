extern WiFiUDP udp;
extern NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);

extern int dia, mes, ano;

extern String hh; // Variável da hora definida do alarme
extern String mm; // Variável do minuto definido do alarme
extern String input; // Lembrete do alarme (Todos os dias/Um dia apenas)

extern unsigned long powerPlugInterval;
extern unsigned long timeUpdateInterval;
extern unsigned long initialSetupDelay;
extern unsigned long coffeeDuration; //Define o tempo em que a cafeteira permanecerá ligada

extern boolean powerPlugIsActive;