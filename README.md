# Conexão MySQL com Linguagem C

Este projeto apresenta um sistema simples de gestão de cadastros, persistindo as informações em um banco de dados MySQL.

O projeto foi desenvolvido utilizando o Microsoft Visual Studio Code, além da suite do MinGW para uso de recursos e também compilação e depuração do projeto.

As versões da linguagem, SGBD e Driver de Conexão estão especificadas a seguir:
 * C Language: C11
 * MySQL: Community 8 Server
 * Driver de Conexão: Connector/C 6.1.11 (Archived Version)

Para que a conexão seja bem sucedida, é necessário seguir as orientações abaixo:

1. Na instalação do MySQL Server, você deve alterar a configuração do SSL, desabilitando-o através dos arquivos arquivo my.ini e data/my.ini (No linux é my.cnf). Inclua as seguinte configuração em ambos arquivos:
    ```
    [mysqld]
    ssl=0
    ```

2. Em seguida, você deve alterar o tipo de senha do usuário para o tipo mysql_native_password. Em meu caso, estou utilizando a senha do usuário root, mas funciona com qualquer senha. Abra o terminal SQL (ou Workbench) e digite a seguinte instrução SQL:
    ```
    ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'root';
    ```

3. Realizada as configurações, baixe a versão mais recente do Connector/C de 32 bits e instale em alguma pasta do computador. Lembre-se de que o Connector/C foi descontinuado pela equipe de desenvolvimento do MySQL, mas, até a versão 8.0 do MySQL Server, o conector funciona corretamente em um SGBD sem SSL. Obs: Não recomendo uso da versão de 64 bits.
https://downloads.mysql.com/archives/c-c/

4. O projeto já possui o c_cpp_properties.json, já que está completamente configurado para funcionar o Microsoft Visual Studio Code. Caso esteja utilizando outra IDE, você deverá configurar as seguintes pastas do Driver Connector/C nas configurações do projeto:
    ```
    include: C:/path/to/mysql-connector-c-6.1.11-win32/include
    lib: C:/path/to/mysql-connector-c-6.1.11-win32/lib
    ```

5. O projeto também está configurado com as instruções de compilação do gcc, dentro do tasks.json. Se estiver criando um projeto do início, você precisará incluir estas instruções de compilação para as bibliotecas do Connector/C:

    ````json
        "args": [
            "-fdiagnostics-color=always",
            "-g",
            "${file}",
            "-o",
            "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "-I",
            "C:/path/to/mysql-connector-c-6.1.11-win32/include",
            "-L",
            "C:/path/to/mysql-connector-c-6.1.11-win32/lib",
            "-llibmysql"
        ]
    ````

6. Após a compilação, será necessário adicionar o arquivo libmysql.dll ou libmysql.dll (Windows/SO) ou libmysqlclient.so (Linux/SO) na mesma pasta de execução do projeto.


Existe um método chamado mysql_ssl_set, responsável por configurar o certificado SSL de um cliente no projeto, mas, até o presente momento, ainda não foi testado.

    ```
    mysql_ssl_set(conn, "./certificados/client-key.pem", "./certificados/client-cert.pem", "./certificados/ca-cert.pem", NULL, NULL);
    ```

Fontes:
* https://scalegrid.io/blog/configuring-and-managing-ssl-on-your-mysql-server/