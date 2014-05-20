#ifndef __EPOLL_PENDING_POOL_
#define __EPOLL_PENDING_POOL_
#include <sys/time.h>
#include "base/public/mutex.h"

class ependingpool {
 public:
  ependingpool();
  ~ependingpool();
  // @brief �Ӿ�������ȡ���Ѿ�����socket
  // @param [out] *handle   : ���ڴ洢���Ӿ��
  // @param [out] *sock     : ���ڴ洢����socket
  // @return  0 �ɹ��� -1ʧ��
  int fetch_item(int * handle, int * sock);

  // @brief �Ӿ�������ȡ���Ѿ�����socket
  // @param [out] *handle   : ���ڴ洢���Ӿ��
  // @param [out] *sock     : ���ڴ洢����socket
  // @param [out] *staytime : ����ڶ����д��ڵ�ʱ��(��λ: ΢�� us)
  // @return  0 �ɹ��� -1ʧ��,
  // -1ʧ�ܿ����ǵ�����stop���а�ȫ�˳�, �ڳ����ʱ��������ж�һ��is_run
  int fetch_item(int *handle, int *sock, long long *staytime);

  // @brief �ر����д��ھ���״̬��socket
  // @return  ���رյ�sock����
  int close_ready_socket();

  // @brief �ȴ�ependingpool��ʣ��Ķ�д�¼�����
  // �����Ҫ������ʹ���˶�д�¼������첽��д��������Ҫ��ȫ�˳���ʱ��
  // ��check_itemҪ��ͬһ���߳���
  // @param [in] timeout   : ��ʱʱ��,����ʱ�����ƣ���ǿ�ƹر����ڴ���д״̬��socket
  // @return  0 �ɹ�
  int waitforstop(int timeout);

  // @brief ͨ��handle ��ȡsocket �����Ľṹָ��(socket��fetch_item��ȡ)
  // @param [in] handle   : int ependingpool���
  // @param [in] arg   : void** ���ؽ�� ��ָ��
  int fetch_handle_arg(int handle, void **arg);

  // @brief �ر����Ӿ��
  // @param [in] handle   : ���Ӿ��
  // @param [in] keep_alive   : false�����Ӿ���رգ�true�����Ӿ�������Ϲر�
  // @return  0 �ɹ��� -1ʧ��
  int reset_item(int handle, bool keep_alive);

  // @brief ��socket�����첽д�������������SOCK_WRITE���õĻص�����
  // д�����ݿ���ͨ����fetch_handle_arg��ȡ��ָ����п���
  // @param [in] handle   : ���Ӿ��
  int write_reset_item(int handle);

  // @brief ��handle��ependingpool���Ƴ��������ر�sock���
  // @param [in] handle   : ���Ӿ��
  int clear_item(int handle);

  // @brief �¼������ص�����������
  // ependingpool�ж���ÿ���������Ӧ��һ��ָ�룬��ʼΪNULL�����û����п���
  // event_callback_t �ڶ�������Ϊvoid ** ���ͣ�ͨ����*arg��ֵ���Դ����û�ָ��
  // �����¼����ں����ķ���ֵ����Ӧ��Ҫ����ο�˵��
  typedef int event_callback_t(int sock, void **arg);
  struct ependingpool_task_t {
    int sock;        // ʹ�õ�sock
    int offset;      // sock��ƫ��
    void *arg;       // ��ֱ��󶨵�����
    void *user_arg;  // �û�����
  };

  // @brief ���ûص�����������
  // @param [in] param   : struct ependingpool_task_t
  typedef int event_callback_ex_t(struct ependingpool_task_t *param);

  // @brief ependingpool���й����з����ĸ����¼��Ķ��壬�������ÿ���¼����ûص�����
  // �ص���������Ϊ @ref event_callback_t
  // ���� sock ��ʾ�����¼���sock
  // ���� void **arg ��ָ����sock�󶨵�ָ�� ��ָ�룬ͨ��*arg ����ʹ��
  // �����¼��Ļص�����˵��:

  // SOCK_ACCEPT �¼� :
  // sock��ʾ���ǽ��м����Ķ˿ڣ���Ҫ�û��Լ�����accept����
  // ��û������SOCK_ACCEPT�����ֻ�Ǽ򵥵�accept��һ��sock
  // ����ֵ: <0 acceptʧ��; >=0 ���ؽ�����sock���

  // SOCK_INT �¼�:
  // ��sock����setsockopt, ���÷Ƕ����Ȳ����������������
  // SOCK_INIT sock��ָ���ʼ�� ,��SOCK_ACCEPT�¼�������
  // ���ʱ����Խ���sock�󶨵�ָ�봫�룬�����г�ʼ��
  // ����ֵ: <0 ��ʼ��ʧ�ܣ�����SOCK_CLEAR�¼����رվ��; =0 ��ʼ���ɹ�������epoll���м���

  // SOCK_LISTENTIMEOUT �¼�:
  // ��epoll������sock��ʱ����, �ص������󣬴���SOCK_CLEAR�¼����ر�sock��
  // ��ʱ��set_conn_timeo����
  // ����ֵ: ���жϷ���ֵ

  // SOCK_READ ���¼�
  // ����Ķ��¼��Ƿ����ڼ�����������֮��������Ҫ��sock�ǷǶ���ģʽ�������ʹ��
  // ����ʵ���첽������
  // ����ֵ:
  // <0 ��ʧ�ܣ��ᴥ��SOCK_CLEAR�¼�
  // =0 ����ȫ����ȡ��ϣ�����sock�������������
  // =1 ���ݶ��ɹ�����û��ȫ����ȡ��ϣ���Ҫ�ٴν��м�����sock�ᱻ�Ż�epoll��
  // =2 ����ȫ����ȡ��ϣ�������sock������������У����Ǵ���SOCK_TODO�¼�������ʵ�ֵ������첽����ģʽ
  // =3 ����SOCK_CLEAR�¼��������رվ��������������Ѿ���Ƴ�ependingpool

  // SOCK_READTIMEOUT sock����ʱ
  // ��SOCK_READ ���¼��󣬷��Ͷ���ʱ���ص������󣬴���SOCK_CLEAR�¼����ر�sock,
  // ��ʱ��set_read_timeo����
  // ����ֵ: ���жϷ���ֵ

  // SOCK_TODO �¼�
  // ��SOCK_READ �¼��ɹ����� 2��ʱ�򱻴���
  // ����ֵ:
  // <0 ����ʧ�ܣ� ����SOCK_CLEAR�¼����رվ��
  // =0 �����ɹ��� ����SOCK_WRITE д�¼�
  // =1 �����ɹ��� ��������SOCK_WRITE д�¼��� sock�Ż�epoll�н��м���������
  // =2 �����ɹ��� ����SOCK_CLEAR�¼��������رվ�������԰Ѿ���Ƴ�ependingpool

  // SOCK_WRITE �¼�
  // ��SOCK_READ������Ҫ�������첽��ʽд���ݣ�sockҲ��Ҫ�ǷǶ���ģʽ
  // ����ֵ:
  // <0 дʧ�ܣ��ᴥ��SOCK_CLEAR�¼�
  // =0 ����ȫ��д��ϣ�����sock���·���epoll�н��м���������(�൱�ڳ�����)
  // =1 ����д�ɹ�����û��ȫ��д�꣬�Ż�epoll�ȴ��´��ټ�������д״̬
  // =2 ����ȫ��д��ϣ��ر�sock���ᴥ��SOCK_CLEAR�¼�(�൱�ڶ�����)
  // =3 ����SOCK_CLEAR�¼��������رվ�������԰Ѿ���Ƴ�ependingpool

  // SOCK_WRITETIMEOUT д��ʱ
  // ��SOCK_WRITE д�¼�������д���ݳ�ʱ, �ص�������SOCK_CLEAR�¼����ر�sock, 
  // ��ʱ��set_write_timeo����
  // ����ֵ: ���жϷ���ֵ

  // SOCK_CLEAR ��ָ���ͷ�
  // ����Ҫclose���ǰ������Ϊ����sock��ָ��, �����ӵ���reset_item��ʱ�򲻻ᴥ�����¼�
  // ����ֵ: ���жϷ���ֵ

  // SOCK_FETCH �����߳�fetch��sock�����¼�
  // �ڵ���fetch_item����ȡ�����õ�sock�󴥷�
  // �벻Ҫ��fetch_item�ڲ����� reset_item �ر�sock, ͨ������ֵ�����ж�
  // ����ֵ:
  // <0 ����sockʧ�ܣ�����SOCK_CLEAR�¼����ر�sock
  // =0 ����sock�ɹ�, �������κ��¼����ⲿ�Ĺ����߳̿���ʹ��ͨ��fetch_item��ȡ�ľ��
  // =1 ����sock�ɹ��������ӣ�����SOCK_CLEAR�¼����ر�sock
  // =2 ����sock�ɹ��������ӣ�������sock�ر��¼���sock�Ż�epoll�м�������
  // =3 ����sock�ɹ�������SOCK_CLEAR�¼�����sock���sock�أ������ر�sock��������Է��������pool��
  // ע�⣬����fetch_item�ķ���ֵ��SOCK_FETCH�¼��ķ���ֵ��ͬ
  // ���ַ�0�ķ���ֵʱ��fetch������sock�ǲ����õ�
  // ��Ϊ����¼������ڹ����߳��У���ע���̰߳�ȫ����

  // SOCK_QUEUEFAIL sock�������ʧ�ܴ���
  // ����ֵ:
  // =0 ����SOCK_CLEAR�¼����ر�sock���
  // =1 ����SOCK_CLEAR�¼��������ر�sock�����������Ƴ�ependingpool

  // SOCK_INSERTFAIL sock����sock��ʧ��ʱ����
  // ����ֵ:
  // ���жϷ���ֵ
  // ע��: SOCK_INSERTFAIL �¼�������SOCK_INIT�¼�֮ǰ
  typedef enum {
    SOCK_ACCEPT = 0,          // ���������¼�
    SOCK_INIT,                // sock��ָ���ʼ���¼�
    SOCK_LISTENTIMEOUT,       // ����sock��ʱ�¼�
    SOCK_READ,                // ���¼�
    SOCK_READTIMEOUT,         // sock�����ݳ�ʱ�¼�
    SOCK_TODO,                // sock�����¼�
    SOCK_WRITE,               // д�¼�
    SOCK_WRITETIMEOUT,        // sockд���ݳ�ʱ�¼�
    SOCK_CLEAR,               // sock��ָ���ͷ��¼�
    SOCK_FETCH,               // �����߳�fetch��sock�����¼�
    SOCK_QUEUEFAIL,           // ����������ʧ��ʱ����
    SOCK_INSERTFAIL,          // ��ependingpool����sockʧ��ʱ����
    SOCK_EVENT_NUM            // �¼����ܸ���
  } ependingpool_event_type;

  // @brief ��ependingpool�����й����з������¼� ���ûص�����
  // ����Ӧ���¼���������ʱ������лص�����
  // ����ص�������Ҫ���ǵ���accept���µ�sock����Ҫ����setsockopt����
  // @return  0 �ɹ��� -1ʧ��
  int set_event_callback(int event, event_callback_t *callback);

  // @brief TODO�¼��ص�,���Ի�ȡoffset
  // @param [in] callback   :  �μ�event_callback_ex_t
  // @param [in] user_arg   :  �û�����, �������event_callback_ex_t��user_arg��
  // @return
  // <0 ����ʧ�ܣ� ����SOCK_CLEAR�¼����رվ��
  // =0 �����ɹ��� ����SOCK_WRITE д�¼�
  // =1 �����ɹ��� ��������SOCK_WRITE д�¼��� sock�Ż�epoll�н��м���������
  // =2 �����ɹ��� ����SOCK_CLEAR�¼��������رվ�������԰Ѿ���Ƴ�ependingpool
  // =3 ʲô����Ҳ����(�൱��feth_item���״̬,����ͨ����callback��ȡ�����ݽ��в���,
  //    ��Ҫ����reset_item��socket���в���
  int set_todo_event_ex(event_callback_ex_t *callback, void *user_arg);

  // @brief ���ü���socket
  // @param [in] lis_fd   : ����socket
  // @return  0�ɹ� -1ʧ��
  int set_listen_fd(int lis_fd);
  // @brief �����Ѿ������еĳ���(Ĭ��:300)
  // @param [in] len   : �Ѿ������еĳ���
  // @return 0 �ɹ� -1 ʧ
  // @note �������̷߳���poolǰ���� , ��Ҫ���������޸�

  int set_queue_len(int len);
  // @brief ���ÿɴ洢socket������(Ĭ��:500)
  // @param [in/out] num   : �ɴ洢socket������
  // @return  0 �ɹ� -1 ʧ��
  // @note �������̷߳���poolǰ���� , ��Ҫ���������޸�

  int set_sock_num(int num);
  // @brief �������ӳ�ʱ(��λ:��, Ĭ��:1s)
  // @param [in] timeo   : ��ʱʱ��
  // @return  0 �ɹ� -1 ʧ��
  // @note  ��������ӳ�ʱ������fetch_item�Ժ��ʱ��

  int set_conn_timeo(int timeo);
  // @brief ����ʹ����SOCK_READ�����, ���ö���ʱ(��λ:��, Ĭ��:1s)
  // @param [in] timeo   : ��ʱʱ��
  // @return  0 �ɹ� -1 ʧ��

  int set_read_timeo(int timeo);
  // @brief ����������SOCK_WRITE�����,����д��ʱ(��λ:��, Ĭ��:1s)
  // @param [in] timeo   : ��ʱʱ��
  // @return  0 �ɹ� -1 ʧ��

  int set_write_timeo(int timeo);
  // @brief ����epollÿ��waitʱ��(��λ:ms,Ĭ��10ms)
  // @param [in] timeo : epoll �ȴ�ʱ��
  // @return  0 �ɹ� -1 ʧ��
  // @note  ���������Ҫ��set_conn_timeo ��ʱ��ҪС����������set_conn_timeo�رվ������ʱ�����

  int set_epoll_timeo(int timeo);
  // @brief ��ȡ��ǰ���еĳ���
  // @return  ���еĳ���

  int get_queue_len();
  // @brief ��ȡ��ǰ���ӳ�ʱʱ��(��)
  // @return  ��ȡ��ǰ���ӳ�ʱʱ��
  // @author baonh

  int get_conn_timeo();
  // @brief ��ȡ��ǰ����ʱʱ��(��)
  // @return  ��ȡ��ǰд��ʱʱ��

  int get_read_timeo();
  // @brief ��ȡ��ǰд��ʱʱ��(��)
  // @return  ��ȡ��ǰд��ʱʱ��

  int get_write_timeo();
  // @brief ��ȡepoll��wait ��ʱ��(ms)
  // @return  epoll��wait ��ʱ��

  int get_epoll_timeo();
  // @brief ��ȡ���е��߳���
  // @return  ���е��߳���

  int get_free_thread();

  typedef enum {
    NOT_USED = 0,       // socket����δʹ��״̬
    READY,              // socket���ھ���״̬���ȴ�����
    READ_BUSY,          // socket���ڶ�״̬
    BUSY,               // socket����æµ״̬, �ڶ����л��߱��߳�fetch��
    WRITE_BUSY          // socket����д״̬
  } sock_status_t;

  // @brief ����socket��״̬
  // ��ȡ�½�������socket
  // ��Ϊ��epool�������̷߳���epoll
  // ��鳬ʱ��socket, socket��ʱ�ᱻ�ر�
  // @return  0 ���ɹ���-1 ʧ��
  int check_item();

  // @brief ��������ependingpoolΪ������״̬
  int restart_run();

  // @brief ependingpool���а�ȫ�˳�,���Խ��is_run()�ж�ependingpool���˳�
  // ʹ��ǰ��ȷ�������߳��Ѿ�����ֹͣ״̬������ͨ���ж�get_free_thread
  // @return  int
  int stop();

  // @brief �ж�ependingpool�Ƿ�������״̬, ��Ҫ���ڽ��̵İ�ȫ�˳���ֹͣ������stop����
  // @return  0 ependingpool���ڹر�״̬ 1 ependingpool���ڿ�����״̬
  // @author baonh
  int is_run();

  // @brief ��sockֱ�ӷ���ependingpool���м���
  // @param [in] sock   : ��Ҫ���м����ľֱ�
  // @param [in] arg    : ��sock�����ָ�룬��ΪNULLʱ�����SOCK_INIT�ص�,
  //                      ���򲻻ᴥ��SOCK_INIT�¼�, ������ر�ʱ���ǻᴥ��SOCK_CLEAR�¼�
  //                      ʹ��ʱ��ע��
  // @param [in] flags  : 0��ʾ��������������0 ��ʾ����д����, Ĭ��Ϊ����������
  // @return >0 Ϊsock��ependingpool�еľ��handle, < 0Ϊʧ��
  // @note ʧ�ܵ�ʱ�򲻻������ر�sock, �����ⲿ��sock�رգ� arg Ҳ�������SOCK_CLEAR�¼�
  //       ��ע���ͷ��ڴ�.
  //       Ĭ������½ӿ����̲߳���ȫ����Ҫʹ��set_insert_item_sock_thrsafe�������á�ʹ��ʱ��ע��
  int insert_item_sock(int sock, void *arg = NULL, int flags = 0);

  // @brief
  // insert_item_sockĬ����������̲߳���ȫ�ģ�set_insert_item_sock_thrsafe(1)
  // ����ʹinsert_item_sock�̰߳�ȫ
  // @param [in] thrsafe   : �ǣ�insert_item_sock�ڲ�������̰߳�ȫ�����ڲ����������̲߳���ȫ
  // @return��0���óɹ���-1����ʧ��
  // @author baonh
  int set_insert_item_sock_thrsafe(int thrsafe);

 private:
  // @brief ��鳬ʱ�����
  // @return  int
  int check_time_out();

  // @brief ��ȡ���е�λ�����ڴ洢��socket
  // @return  ���е�λ�þ��
  int get_offset();

  // @brief ��socket����
  // @param [in] offset   :�����е�λ�þ��������get_offset��ȡ
  // @return  0 ����ɹ���-1����ʧ��
  // @note  ����ʧ�ܵ�ͬʱ���socket�ر�
  int insert_item(int offset);

  // @brief ���Ѿ�׼��������socket�������������
  // @param [in] offset   : �Ѿ�����socket��offset���
  // @return  0 ����ɹ���-1����ʧ��
  // @note  �������ʧ�ܣ���������close�������´��ٴν��м��
  int queue_in(int offset);

  // @brief ����в�����صĺ���
  // @param [in/out] queue_size   : int
  int queue_create(int queue_size);
  int queue_push(int val);
  int queue_pop(int* val);
  int queue_empty();
  int queue_full();
  int queue_destroy();
  // @brief accept ��set_listen_fd���õľ��
  // @return  �µ����Ӿ��
  int accept_sock();
  // @brief epoll�ȴ�
  // @param [in] timeout   : �ȴ�ʱ��
  int pool_epoll_wait(int timeout);
  int pool_epoll_add(int socket, int fd, int event);
  int pool_epoll_del(int socket, int fd);
  int pool_epoll_mod(int socket, int fd, int event);
  int pool_epoll_offset_add(int offset, int event);
  int pool_epoll_offset_del(int offset);
  int pool_epoll_offset_mod(int offset, int event);
  void do_read_event(int offset);
  void do_write_event(int offset);

  typedef struct _sock_item {
    int sock_status;           // socket״̬
    int sock;                  // �洢��socket
    int last_active;           // �����ʱ��,�жϳ�ʱʹ��
    void *arg;
    struct timeval queue_time; // socket������е�ʱ��
  } sock_item_t;

  int m_sock_num; // �洢��socket��Ŀ
  sock_item_t *m_ay_sock; // ���ڴ��socket�Ŀռ�
  int m_sock_len; // ʵ�ʴ洢��socket��Ŀ
  int m_least_last_update; // ���д���ready״̬socket ���ʱ�䡡��С��ʱ��
  int m_queue_len; // ������󳤶�
  int *m_ay_ready; // ��������
  int m_get;  // ����ͷ
  int m_put;  // ����β
  struct epoll_event *m_ay_events;      // �洢epoll���¼�����

  int m_free_thr;         // ��ȡ�ȴ���ȡ���ӵ��߳���
  int m_conn_timeo;       // ���ӳ�ʱʱ��
  int m_read_timeo;       // ����ʱ
  int m_write_timeo;      // д��ʱ
  int m_min_timeo;
  int m_epfd;              // epoll ���
  int m_ep_timeo;          // epoll �ȴ�ʱ��
  int m_insert_item_sock_thrsafe;
  int m_listen_fd;        // �������
  event_callback_t * m_callbacklist[SOCK_EVENT_NUM];
  void *m_todo_event_user_arg;
  event_callback_ex_t *m_todo_event_callback;
  int pool_run;
  int m_read_socket_num;
  int m_write_socket_num;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_condition;
  pthread_mutex_t m_insert_sock;
};

#endif //__EPOLL_PENDING_POOL_
