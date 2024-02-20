// Send a Message


#pragma once


inline void sendMsg(int msgID, WPARAM wParam, LPARAM lParam)
                                                    {AfxGetMainWnd()->PostMessage(msgID, wParam, lParam);}


