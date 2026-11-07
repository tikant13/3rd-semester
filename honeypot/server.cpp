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

// Задаём порт
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

// Баннер того как сервер представится клиенту 
if (ssh_bind_options_set(bind, SSH_BIND_OPTIONS_BANNER, "OpenSSH_8.9p1 Ubuntu-3ubuntu0.10") !=SSH_OK){
    std::cerr << "ошибка баннера: " << ssh_get_error(bind) << std::endl;
    ssh_bind_free(bind); // деструктор
    ssh_finalize();
    return 1;
}

//Начинаем слушать порт
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
        std::cerr << "не удалось создать session " << std::endl;
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
        std::cout << "Клиент: " << clientBanner << std::endl;
    } else {
        std::cout << "Нет версии клиента " << std::endl;
    }
    ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD | SSH_AUTH_METHOD_PUBLICKEY); // какие способы входа предлагать
    int attempts = 6; // количество попыток


    //обработка клиентов
    while (true){
        ssh_message msg = ssh_message_get(session); // ждём и записываем сообщение
        if (msg == nullptr){
            break;
        }

        int type = ssh_message_type(msg); // записываем тип
        int subtype = ssh_message_subtype(msg); // записываем подтип
        std::cout << "использует тип: " << type << "    использует подтип: " << subtype << std::endl;

        if (type == SSH_REQUEST_AUTH && subtype == SSH_AUTH_METHOD_PASSWORD){
            const char* user = ssh_message_auth_user(msg); // уже устаревщие, но рабочие
            const char* password = ssh_message_auth_password(msg); // уже устаревщие, но рабочие
            if (user == nullptr || password == nullptr){
                std::cout << "Ошибка в логине или пароле" << std::endl;
            } else {
                 std::cout << "user: " << user << "    password: " << password << std::endl;
            }
            --attempts;
            if (attempts == 0){
                ssh_message_reply_default(msg); // отвечаем по умолчанию (отказом)
                ssh_message_free(msg); // освобождаем память
                break;
            }
        }

        ssh_message_reply_default(msg); // отвечаем по умолчанию (отказом)
        ssh_message_free(msg); // освобождаем память

    }

    ssh_disconnect(session);
    ssh_free(session);
} 

ssh_bind_free(bind);
ssh_finalize(); // в конце программы 
}