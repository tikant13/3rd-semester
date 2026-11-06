#include <iostream>
#include <libssh/libssh.h>
#include <libssh/server.h>

int main(){
ssh_init(); // инициализация библиотеки 

ssh_bind bind = ssh_bind_new(); // дверь сервера: порт, ключ хоста, баннер
if (bind == nullptr) {
    std::cerr << "не удалось создать bind" << std::endl;
    ssh_finalize();
    return 1;
}

// задаём порт
if (ssh_bind_options_set(bind, SSH_BIND_OPTIONS_BINDPORT_STR, "2222") != SSH_OK) {
    std::cerr << "ошибка порта: " << ssh_get_error(bind) << std::endl;
    ssh_bind_free(bind); // деструктор
    ssh_finalize();
    return 1;
}

// Путь к секретному ключу сервера
if (ssh_bind_options_set(bind, SSH_BIND_OPTIONS_HOSTKEY, "ssh_host_ed25519_key") !=SSH_OK){
    std::cerr << "ошибка ключа: " << ssh_get_error(bind) << std::endl;
    ssh_bind_free(bind); // деструктор
    ssh_finalize();
    return 1;
}

// баннер того как сервер представится клиенту 
if (ssh_bind_options_set(bind, SSH_BIND_OPTIONS_BANNER, "OpenSSH_8.9p1 Ubuntu-3ubuntu0.10") !=SSH_OK){
    std::cerr << "ошибка баннера: " << ssh_get_error(bind) << std::endl;
    ssh_bind_free(bind); // деструктор
    ssh_finalize();
    return 1;
}

//начинаем слушать порт
if (ssh_bind_listen(bind) != SSH_OK){
    std::cerr << "ошибка запуска сервера: " << ssh_get_error(bind) << std::endl;
    ssh_bind_free(bind); // деструктор
    ssh_finalize();
    return 1;
} 

std::cout << "Сервер слушает порт 2222" << std::endl;

while (true){
    ssh_session session = ssh_new(); // создаём новую сессию для клиента
    if (session == nullptr){
        std::cerr << "не удалось создать session" << std::endl;
        ssh_bind_free(bind);
        ssh_finalize();
        return 1;
    }
    if (ssh_bind_accept(bind, session) != SSH_OK){ // ждём подключения
        std::cerr << "Проблемы с подключением: " << ssh_get_error(bind) << std::endl;
        ssh_free(session);
        continue;
    }
    
    if(ssh_handle_key_exchange(session) != SSH_OK){ // обмен версиями и ключами
        std::cerr << "Проблемы с клиентом: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session); // закрываем соединение 
        ssh_free(session);
        continue;
    }

    const char* clientBanner = ssh_get_clientbanner(session); // записываем баннер клиента
    if (clientBanner != nullptr){
        std::cout << clientBanner << std::endl;
    } else {
        std::cout << "Нет версии клиента " << std::endl;
    }
    ssh_disconnect(session);
    ssh_free(session);
} 

ssh_bind_free(bind);
ssh_finalize(); // в конце программы 
}