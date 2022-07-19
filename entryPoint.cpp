#include "includes.h"
#include "db_operations.h"

std::string mysqlcppconn_test(std::string tag)
{
	std::string request_str = "tag=\"" + tag + "\"";
	try
	{
		mysqlx::Session session("mysqlx://root:1237@localhost");
		mysqlx::Schema sch = session.getSchema("tourist_places1");
		mysqlx::Table tab = sch.getTable("places", true);
		mysqlx::RowResult result = tab.select().where(request_str).execute(); //data for quick testing: "tag = \"ћурманска€область\""
		printf("\nrows count: %d\n", result.count());
		mysqlx::Value value = result.fetchOne().get(1);
		printf("\nrows count: %d\n", result.count());
		return (std::string)value;
	}
	catch (const mysqlx::Error& err)
	{
		printf("\nmysqlx ERROR: %s\n", err.what());
		system("pause");
	}
	catch (std::exception& ex)
	{
		printf("\nSTD EXCEPTION: %s\n", ex.what());
		system("pause");
	}
	catch (const char* ex)
	{
		printf("\nEXCEPTION: %s\n", ex);
		system("pause");
	}
}

void filesystem_test()
{
	const std::filesystem::path pathObject("Photos\\13230345602488322");
	for (auto const& dir_entry : std::filesystem::directory_iterator(pathObject))
	{
		std::cout << dir_entry << '\n';
	}
	system("pause");
}

int main()
{
	///setlocale(LC_ALL, "rus");
	
	std::string endpoint = "http://localhost:8080";
	web::http::experimental::listener::http_listener lstnr(get__string_t(endpoint));
	lstnr.support(web::http::methods::GET, [](web::http::http_request req) {
		printf("\n\nRequest catched, processing... ");

		if (req.request_uri().path() == get__string_t("/favicon.ico"))
		{
			printf("it's favicon.ico \"GET request\", backing data... ");

			FILE* iconFile;
			fopen_s(&iconFile, "favicon.ico", "rb");

			int currentChar = 0;
			std::string body = ""; //test
			do {
				currentChar = fgetc(iconFile);
				body += (char)currentChar;
			} while (currentChar != EOF);
			fclose(iconFile);
			printf("\n\nicon binary data:\n%s\n\n", body.c_str());

			printf("icon data ready, fetching... ");
			web::http::http_response response(200);
			response.headers().set_content_type(get__string_t("image/vnd.microsoft.icon"));
			response.set_body(body);
			req.reply(response);

			printf("Data fetched!");
			return;
		}

		auto queryMap = web::uri::split_query(req.request_uri().query());
		utility::string_t query_type = queryMap.at(get__string_t("query-type"));

		web::json::value bodyData;
		utility::string_t arg_encoded = queryMap.at(get__string_t("arg"));
		std::string arg = utility::conversions::to_utf8string(web::uri::decode(arg_encoded));
		if (query_type == get__string_t("places")) bodyData = db::getData_fromRegion(arg);
		else if (query_type == get__string_t("images")) bodyData = db::getImages_fromID(arg);
		else bodyData[get__string_t("ErrorMsg")] = web::json::value::string(get__string_t("incorrect query-type!"));

		printf("bodyData created, fetching... ");

		web::http::http_response response(200);
		response.headers().set_content_type(get__string_t("application/json")); ///; charset=UTF-8
		response.set_body(bodyData);
		req.reply(response);

		printf("Data fetched!");
		});
	lstnr.open();
	printf("Web server up and listens at %s\n", endpoint.c_str());
	system("pause");
	lstnr.close();
}