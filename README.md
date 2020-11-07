# Лабораторная работа №8

## Задание

К **suggest** сервису, который обрабатывает **POST** запросы из лабораторной работы №7, реализовать клиент.
Клиент отправляет **POST** запрос следующего формата:
```json
{
  "input": "hel"
}
```
В поле `input` находится текст, который предназначен для обработки сервисом. В случае, если сервису нечего предложить клиенту, то он вернет пустой массив.
Клиент обращается к сервису с именем пользователя (без пароля). Все соединения инициируются клиентом, где клиент запрашивает ответ от сервиса. Ответ от сервиса необходимо вывести с использованием стандартных потоков.

## Рекомендации

* для реализации HTTP клиента могут быть использованы готовые библиотеки
* в пакетном менеджере hunter есть несколько [библиотек](https://hunter.readthedocs.io/en/latest/packages/networking.html) для работы с сетью, например, [Boost.Beast](https://hunter.readthedocs.io/en/latest/packages/pkg/Beast.html#pkg-beast)
* у хороших библиотек есть наглядные примеры работы с этими библиотеками, например, простой [синхнорнный http клиент на основе Boost.Beast](https://github.com/boostorg/beast/blob/develop/example/http/client/sync/http_client_sync.cpp)

## Links

- [Boost.Beast](https://github.com/boostorg/beast)
- [Vinnie Falco: Boost.Beast](https://vinniefalco.github.io/beast/)
- [POST](https://ru.wikipedia.org/wiki/POST_(HTTP))
