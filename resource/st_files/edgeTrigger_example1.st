result=( state & !lastState | !state & lastState );
lastState=state;
