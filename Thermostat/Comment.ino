//char login[] = "login";
//char password[] = "password";

/*String auth_update(char login[], char password[]) {
  // Строка для будущего хэша
  String hash = strcat(strcat(login, ":"),password);
  int buf_size = hash.length()+1;
  char buf_char[buf_size];
  // Переводим строку в char для дальнейшей обработки base64
  hash.toCharArray(buf_char, buf_size);
  char encoded[base64_enc_len(buf_size-1)];
  // Получаем хэш для авторизации
  base64_encode(encoded, buf_char, buf_size-1);
  // Формируем полную строку поиска "Authorization: Basic <BASE64-HASH>" чтобы не могли подсунуть хэш в GET или POST
  return "Authorization: Basic " + String(encoded);
}*/

// формування стандартного http заголовку
/*client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 5");  // refresh the page automatically every 5 sec
client.println();
//початок сторінки
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<meta http-equiv='content-type' content='text/html; charset=KOI8-U'>");

client.print("Start time: ");
client.print(millis());
client.println("<br />");

client.print("Температура 1: ");
client.print(Temp1, 5);
client.println("<br />");
client.print("Temp 2: ");
client.print(Temp2, 5);
client.println("<br />");
client.print("Temp 3: ");
client.print(Temp3, 5);
client.println("<br />");
client.println("</html>");
break;
}*/
