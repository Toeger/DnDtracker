#ifndef UI_UTILITY_H
#define UI_UTILITY_H

#include <QObject>
#include <functional>

class QEvent;

namespace ui_utility {
	//usage: some_qobject->installEventFilter(new ui_utility::Event_filter(some_qobject, [](QObject *object, QEvent *event) { return true; }));
	//for the callback return true if the event should be filtered out or false to let it be handled normally
	class Event_filter : public QObject {
		Q_OBJECT
		public:
		Event_filter(QObject *parent, std::function<bool(QObject *, QEvent *)> function);
		bool eventFilter(QObject *object, QEvent *event) override;

		private:
		std::function<bool(QObject *, QEvent *)> callback;
	};
}

#endif // UI_UTILITY_H