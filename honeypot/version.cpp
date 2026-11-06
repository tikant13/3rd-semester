#include <iostream>
#include <libssh/libssh.h>

using namespace std;

int main(){
    cout << ssh_version(0) << endl;
    return 0;
}

/*
int  ssh_init();                     // в начале программы
int  ssh_finalize();                 // в конце

ssh_bind ssh_bind_new();
int  ssh_bind_options_set(ssh_bind b, enum ssh_bind_options_e option, const void* value);
int  ssh_bind_listen(ssh_bind b);                 // начать слушать порт
int  ssh_bind_accept(ssh_bind b, ssh_session s);  // ЖДЁТ клиента и связывает его с s
void ssh_bind_free(ssh_bind b);

ssh_session ssh_new();
int  ssh_handle_key_exchange(ssh_session s);      // обмен версиями и ключами — всё, что nc не умел
const char* ssh_get_clientbanner(ssh_session s);  // строка версии клиента
void ssh_disconnect(ssh_session s);
void ssh_free(ssh_session s);

const char* ssh_get_error(void* obj);             // текст последней ошибки (bind или session)
*/