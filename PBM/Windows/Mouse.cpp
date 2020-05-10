#include "../stdafx.h"
#include "Mouse.h"

void Mouse::Init(HWND hWnd)
{
	m_HWnd = hWnd;
}

void Mouse::DisplayCursor()
{
	//cout << "display cursor before" << m_CursorDisplayCount << endl;
	if (m_CursorDisplayCount >= 0)
		return;
	ShowCursor(true);
	m_CursorDisplayCount++;
	//cout << "display cursor after" << m_CursorDisplayCount << endl;
}

void Mouse::HideCursor()
{
	//cout << "hide cursor before" << m_CursorDisplayCount << endl;
	if (m_CursorDisplayCount < 0)
		return;
	ShowCursor(false);
	m_CursorDisplayCount--;
	//cout << "hide cursor after" << m_CursorDisplayCount << endl;
}

void Mouse::MoveUpdate(int client_x, int client_y)
{
	switch (m_TrackingState) {
	case TRACKING_PREPARE:
		TrackingPrepare(client_x, client_y);
		m_TrackingState = TRACKING;
		break;
	case TRACKING:
		Tracking(client_x, client_y);
		break;
	}
}

void Mouse::WheelUpdate(double wheelDistance)
{
	m_MouseWheel += wheelDistance;
}

void Mouse::ResetWheel()
{
	m_MouseWheel = 0;
}

double Mouse::WheelDistance()
{
	return m_MouseWheel;
}

void Mouse::TrackingPrepare(int x, int y)
{
	RECT winRect;
	if (GetWindowRect(m_HWnd, &winRect))
	{
		POINT center{ (winRect.left + winRect.right) / 2 , (winRect.top + winRect.bottom) / 2 };
		//m_MouseInput.mi.dx = 65535 * center.x / (double)GetSystemMetrics(SM_CXSCREEN);
		//m_MouseInput.mi.dy = 65535 * center.y / (double)GetSystemMetrics(SM_CYSCREEN);
		m_ActualScreenCoord = m_ActualClientCoord = center;
		SetCursorPos(center.x, center.y);
		ScreenToClient(m_HWnd, &m_ActualClientCoord);
		m_VirtualCoord = { 0, 0 };
		cout << "TrackingPrepare Center: " << center.x << ", " << center.y << endl;
	}
	else
		cout << "GetWindowRect Error, Code:" << GetLastError() << endl;
}

void Mouse::Tracking(int client_x, int client_y)
{
	m_VirtualCoord.x += client_x - m_ActualClientCoord.x;
	m_VirtualCoord.y += client_y - m_ActualClientCoord.y;
	SetCursorPos(m_ActualScreenCoord.x, m_ActualScreenCoord.y);
	//cout << "Tracking Input Coord: " << client_x << ", " << client_y << endl;
	//cout << "Tracking Actual Client Coord: " << m_ActualClientCoord.x << ", " << m_ActualClientCoord.y << endl;
	//cout << "Tracking Virtual Coord: " << m_VirtualCoord.x << ", " << m_VirtualCoord.y << endl;
}

void Mouse::BeginTracking()
{
	m_TrackingState = TRACKING_PREPARE;
	HideCursor();
}

void Mouse::ResumeTracking()
{
	m_TrackingState = TRACKING_PREPARE;
	HideCursor();
}

void Mouse::PauseTracking()
{
	m_TrackingState = TRACKING_PAUSE;
	DisplayCursor();
}

void Mouse::EndTracking()
{
	m_TrackingState = TRACKING_STOPED;
	DisplayCursor();
}