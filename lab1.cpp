// #include "sysinclude.h"
#include <deque>

extern void SendFRAMEPacket(unsigned char *pData, unsigned int len);

#define WINDOW_SIZE_STOP_WAIT 1
#define WINDOW_SIZE_BACK_N_FRAME 4

typedef enum
{
	data,
	ack,
	nak
} frame_kind;
typedef struct frame_head
{
	frame_kind kind;		 // ֡����
	unsigned int seq;		 // ���к�
	unsigned int ack;		 // ȷ�Ϻ�
	unsigned char data[100]; // ����
};
typedef struct frame
{
	frame_head head;   // ֡ͷ
	unsigned int size; // ���ݵĴ�С
};

std::deque<frame *> appending_queue;
int send_win = 0;
int receive_win = 0;

/*
* ͣ��Э����Ժ���
*/
int stud_slide_window_stop_and_wait(char *pBuffer, int bufferSize, UINT8 messageType)
{
	frame *frame = new frame;
	switch (messageType)
	{
	case MSG_TYPE_SEND:
		*frame = *((frame *)pBuffer);
		appending_queue.push_back(frame);
		if (send_win < WINDOW_SIZE_STOP_WAIT)
		{
			SendFRAMEPacket((unsigned char *)pBuffer, bufferSize);
			send_win++;
		}
		break;

	case MSG_TYPE_RECEIVE:
		*frame = *((frame *)pBuffer);
		unsigned int ack = ntohl(frame->head.ack);
		frame *local = appending_queue.front();
		if (ntohl(local->head.seq) == ack)
		{
			appending_queue.pop_front();
			send_win--;
			if (appending_queue.empty() == false)
			{
				local = appending_queue.front();
				SendFRAMEPacket((unsigned char *)local, local->size);
				send_win++;
			}
		}
		break;

	case MSG_TYPE_TIMEOUT:
		*frame = appending_queue.front();
		unsigned int seq = ntohl(frame->head.seq);
		unsigned int timeout_seq = ntohl(*(unsigned int *)pBuffer) if (seq == timeout_seq)
		{
			SendFRAMEPacket((unsigned char *)frame, frame->size);
		}
		break;

	default:
		// not supposed to reach here
		return -1;
	}
	return 0;
}

/*
* ����n֡���Ժ���
*/
int stud_slide_window_back_n_frame(char *pBuffer, int bufferSize, UINT8 messageType)
{
	return 0;
}

/*
* ѡ�����ش����Ժ���
*/
int stud_slide_window_choice_frame_resend(char *pBuffer, int bufferSize, UINT8 messageType)
{
	return 0;
}
