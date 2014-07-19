trabalho-ippd
=============

Compilação e uso:
make : compila o programa com o uso de threads
./trabalho_ippd <n_Threads> <n_Entries>
make dependencias:
    instala as bibliotecas necessarias, a biblioteca SFML e um pouco “chata” para instalar entao inclui ela no projeto (por isso há algumas pastas bin, include e share), esse comando instala (no ubuntu) a biblioteca que falta, por apt-get, porém devido a versao compilada da biblioteca foi utilizada uma versao antiga (no ubuntu 13.10) da biblioteca libglew, inclui tambem o comando utilizado na minha maquina para linkar a versao nova para a antiga. talvez haja necessidade de alterar o comando para se adequar ao sistema.
make clean: limpa a compilação
    
