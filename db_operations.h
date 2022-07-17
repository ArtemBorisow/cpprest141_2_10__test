#include "includes.h"

namespace db
{
	web::json::value getData_fromRegion(std::string regionName)
	{
		std::string request_str = "tag=\"" + regionName + "\"";
		try
		{
			mysqlx::Session session("mysqlx://root:1237@localhost");
			mysqlx::RowResult result = session.getSchema("tourist_places1").getTable("places", true).select().where(request_str).execute();

			web::json::value jsonArray;

			int index = 0;
			while (result.count() != 0)
			{
				mysqlx::Row currentRow = result.fetchOne();

				jsonArray[index][get__string_t("id")] = web::json::value::number((int)currentRow.get(0));
				jsonArray[index][get__string_t("name")] = web::json::value::string(get__string_t((std::string)currentRow.get(1)));
				jsonArray[index][get__string_t("latitude")] = web::json::value::number((double)currentRow.get(4));
				jsonArray[index][get__string_t("longitude")] = web::json::value::number((double)currentRow.get(5));

				index++;
			}

			return jsonArray;
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

	web::json::value getImages_fromID(std::string ID)
	{
		std::string request_str = "id=" + ID;
		try
		{
			mysqlx::Session session("mysqlx://root:1237@localhost");
			mysqlx::Row result = session.getSchema("tourist_places1").getTable("places", true).select().where(request_str).execute().fetchOne();
			std::string grouped_id = (std::string)result.get(6);

			web::json::value jsonArray;

			int index = 0;
			const std::filesystem::path dir("Photos\\" + grouped_id);
			for (auto const& dir_entry : std::filesystem::directory_iterator(dir))
			{
				std::vector<unsigned char> imageByteVector;

				FILE* imageFile;
				fopen_s(&imageFile, dir_entry.path().string().c_str(), "rb");

				int currentChar = 0;
				do {
					currentChar = fgetc(imageFile);
					imageByteVector.push_back(currentChar);
				} while (currentChar != EOF);
				fclose(imageFile);

				utility::string_t image_base64 = utility::conversions::to_base64(imageByteVector);

				jsonArray[index] = web::json::value::string(image_base64);
				index++;
			}

			return jsonArray;
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
}
