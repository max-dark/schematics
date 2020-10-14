# регулировка скооростей линии лесопиления

## адреса переменных

```
DB1112 DBW    0	 Int	B Speed1/Am_0
DB1112 DBW   44	 Int	DWS speed_2
DB1112 DBW    4	 Int	FBS1 speed
DB1112 DBW   16	 Int	FBS2 speed_0
DB1113 DBW   22	 Int	Lnta speed_3
DB1113 DBW   20	 Int	Othod speed_4
DB1112 DBW   28	 Int	PKA1 speed_1
DB1112 DBW   60	 Int	Sab Speed1
DB1111 DBX 30.0	Bool	Sab Speed1/Am
DB1113 DBW    4	 Int	Sab Speed1/Am_0
DB1113 DBW   10	 Int	Sab Speed1/Am_1
DB1112 DBW   58	 Int	Sab Speed1/Am_2
```



* "Sab speed1" db1112.dbw60	int	"Общая скорость"
* "Sab speed1/Am" db1111.dbx30.0 bool "Общая скорость / активация"

* "B Speed1/Am_0" DB1112.DBW0	Int "Подача"
* "FBS1 speed" DB1112.DBW4	Int "ФБС1"
* "FBS2 speed_0" DB 1112 DBW 16	Int "ФБС2"
* "PKA1 speed_1" DB 1112 DBW 28	Int "ПКА"
* "DWS speed_2" DB 1112 DBW 44	Int "многопил"
* "Othod speed_4" DB 1113 DBW 20	Int "Скорость отвода доски" ???
* "Lnta speed_3" DB 1113 DBW 22	Int "Скорость ленты"
* "Sab speed1/Am_0" DB 1113 DBW 4	Int "Скорость сброса доски"
* "Sab speed1/Am_1" DB 1113 DBW 10	Int "Скорость сброса доски на сортировку"
* "Sab speed1/Am_2" DB 1112 DBW 58	Int "Скорость рольганга"
