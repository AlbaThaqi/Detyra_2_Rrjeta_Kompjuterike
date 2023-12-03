# Detyra_2_Rrjeta_Kompjuterike
## Përshkrim i përmbledhur i programit
Komunikimi klient-server me UDP është implementuar në gjuhën programuese C++ në kuadër të lëndës Rrjetat Kompjuterike.
UDP është një protokoll i transportit connectionless i cili nuk siguron garanci për dorëzim të të dhënave apo ruajtje të renditjes së tyre mes klientit dhe serverit.

Ky implementim i programit përnbushë të gjitha kërkesat e dhëna. Poashtu në rastin ku kemi lejuar konektimin e deri 4 klientëve (për katër anëtarët e grupit), kemi kontrolluar në rast se kur tenton një klient i pestë të kyçet në server, serveri mos ta lejojë. Klienti ka mundësi të bëjë më shumë se një kërkesë. Në rastin tonë klienti mund të jetë admin apo read-only user.
Admini ka write, read dhe execute permissions, ndërsa user-i read-only ka vetëm read permissions. P.sh. përmes write permissions admini ka të drejtë të shkruaj në çfarëdo fajlli që gjendet në server; përmes read permissions admini ka të drejtë të lexojë çfarëdo fajlli që gjendet në server, ndërsa përmes execute permissions admini ka të drejtë të ekzekutojë çfarëdo komande, p.sh.: mkdir new-folder, dir, python file.py, etj dhe read-only users mund vetëm të lexojnë fajlla të serverit. Në rast se një read-only user tenton të ekzekutojë apo të shkruajë diçka, serveri nuk e lejon.

Serveri operon në portin 1205 dhe pranon koneksionet nga të gjithë klientët e lidhur në të njëjtin rrjet me serverin.

## Ekzekutimi i programit të implementuar
Programi është implementuar në atë mënyrë që klienti të mund të autentikohet si admin user ose si read-only user duke dhënë kredencialet e caktuara të kërkuara nga serveri.

Së pari ekzekutohet serveri dhe pastaj ekzekutohet klienti. Serveri është për shembull njëri nga llaptopët dhe klientët llaptopët tjerë. IP adresa tek klienti duhet të përshtatet varësisht prej IP adresës së serverit. Pas hapjes së kodit në Visual Studio, klikohet **Local Windows Debugger**.

Më poshtë është një shembull i ekzekutimit ku klienti është admin:

![image](https://github.com/AlbaThaqi/Detyra_2_Rrjeta_Kompjuterike/assets/112955653/10809e92-d258-4867-9527-c6a002830484)

![image](https://github.com/AlbaThaqi/Detyra_2_Rrjeta_Kompjuterike/assets/112955653/b92b6ee5-c424-4ef3-b3ce-0d657008cda7)
![image](https://github.com/AlbaThaqi/Detyra_2_Rrjeta_Kompjuterike/assets/112955653/75713d3b-9fd6-48b2-a1bc-b2f0f00ea74b)

Më poshtë është një shembull i ekzekutimit ku klienti është read-only user:

![image](https://github.com/AlbaThaqi/Detyra_2_Rrjeta_Kompjuterike/assets/112955653/d2be719f-1e0e-4ae3-b69a-832024f150da)

## Libraritë e nevojshme
### Libraritë e serverit:
- iostream
  
- cstring
  
- winsock2.h
  
- ws2tcpip.h
  
- thread
  
- vector
  
- fstream
  
- sstream
  
- queue
  
### Libraritë e klientit:
- iostream
  
- cstring
  
- winsock2.h
  
- ws2tcpip.h

## Anëtarët e grupit:
- Alba Thaqi
  
- Albatin Totaj
  
- Ajshe Selmani
  
- Aladin Bajra
