#include "ui_utility.h"

ui_utility::Event_filter::Event_filter(QObject *parent, std::function<bool(QObject *, QEvent *)> function)
	: QObject(parent)
	, callback({std::move(function)}) {}

bool ui_utility::Event_filter::eventFilter(QObject *object, QEvent *event) {
	return callback(object, event);
}