#pragma once

enum ETrackingState {
	TRACKING_PREPARE,
	TRACKING,
	TRACKING_PAUSE,
	TRACKING_STOPED
};

class Mouse
{
public:
	void Init(HWND hWnd);
	void DisplayCursor();
	void HideCursor();
	void MoveUpdate(int x, int y);
	void WheelUpdate(double wheelDistance);
	void ResetWheel();
	double WheelDistance();
	inline bool IsTrackingStarted() const;  // 除了STOPED其他状态都是Started
	inline bool IsTracking() const;
	inline bool CheckTrackingState(ETrackingState state) const;
	void BeginTracking();
	void ResumeTracking();
	void PauseTracking();
	void EndTracking();
	// 返回鼠标移动的距离
	// Tracking状态下才有意义，因为其他状态不会记录Coord。
	inline POINT VirtualCoord() const;
protected:
	void TrackingPrepare(int x, int y);
	void Tracking(int x, int y);
protected:
	double m_MouseWheel;
	ETrackingState m_TrackingState = TRACKING_STOPED;
	int m_CursorDisplayCount = 0;
	POINT m_VirtualCoord;
	POINT m_ActualScreenCoord;
	POINT m_ActualClientCoord;
	HWND m_HWnd;
	friend class MyWindow;
};

bool Mouse::IsTrackingStarted() const
{
	return !CheckTrackingState(TRACKING_STOPED);
}

bool Mouse::IsTracking() const
{
	return CheckTrackingState(TRACKING);
}

bool Mouse::CheckTrackingState(ETrackingState state) const
{
	return m_TrackingState == state;
}

inline POINT Mouse::VirtualCoord() const
{
	return m_VirtualCoord;
}
