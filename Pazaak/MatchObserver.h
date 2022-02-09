
#ifndef MATCHOBSERVER_H_
#define MATCHOBSERVER_H_

class MatchObserver {
public:
	virtual void notifySetLost() = 0;
	virtual void notifySetDraw() = 0;
	virtual void notifySetWon() = 0;
};

#endif // !MATCHOBSERVER_H_
