#include <iostream>  // need this header file to support the C++ I/O system
#include "Source.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "containerstream.h"
#include "filestream.h"
#include "http_client.h"

#include "producerconsumerstream.h"
#include <iostream>
#include "json.h"
#include <sstream>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
using namespace cv;
using namespace std;
// basic-http-client.cpp
using namespace ::pplx;
using namespace utility;
using namespace concurrency::streams;

using namespace web::http;
using namespace web::http::client;
#include <http_client.h>
#include <iostream>
#include <json.h>
#include "timer.h"
using namespace web;
using namespace web::http;
using namespace web::http::client;
#include <chrono>
#include <thread>
vector <string> streamList;
int timer = 99;

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void DisplayJSONValue(json::value v)
{



	if (!v.is_null())
	{
		// Loop over each element in the object
		for (auto iter = v.cbegin(); iter != v.cend(); ++iter)
		{
			// It is necessary to make sure that you get the value as const reference
			// in order to avoid copying the whole JSON value recursively (too expensive for nested objects)
			const json::value &key = iter->first;
			const json::value &value = iter->second;

			if (value.is_object() || value.is_array())
			{
				// We have an object with children or an array
				if ((!key.is_null()) && (key.is_string()))
				{
					std::wcout << L"Parent: " << key.as_string() << std::endl;
				}
				// Loop over each element in the object by calling DisplayJSONValue
				DisplayJSONValue(value);
				if ((!key.is_null()) && (key.is_string()))
				{
					std::wcout << L"End of Parent: " << key.as_string() << std::endl;
				}
			}
			else
			{
				// Always display the value as a string
				std::wcout << L"Key: " << key.as_string() << L", Value: " << value.to_string() << std::endl;
				if (key.as_string() == L"template")
				{
					std::string str = ::utility::conversions::to_utf8string(value.as_string());
					streamList.push_back(str);
				}

			}
		}
	}
}
pplx::task<void> RequestJSONValueAsync()
{
	// TODO: To successfully use this example, you must perform the request  
	// against a server that provides JSON data.  
	// This example fails because the returned Content-Type is text/html and not application/json.
	http_client client(L"https://api.twitch.tv/kraken/streams?game=Dota+2");
	return client.request(methods::GET).then([](http_response response) -> pplx::task<json::value>
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}

		// Handle error cases, for now return empty json value... 
		return pplx::task_from_result(json::value());
	})
		.then([](pplx::task<json::value> previousTask)
	{
		try
		{
			//string_t ss = (U("preview"));
			const json::value& v = previousTask.get();
			DisplayJSONValue(v);
			// Perform actions here to process the JSON value...
			//const json::value& s = v.parse(U("swag"));
			//std::string str = ::utility::conversions::to_utf8string(v.as_string());
			string s = "sw";
		}
		catch (const http_exception& e)
		{
			// Print error.
			wostringstream ss;
			ss << e.what() << endl;
			wcout << ss.str();
		}
	});

	/* Output:
	Content-Type must be application/json to extract (is: text/html)
	*/
}
/*pplx::task<void> HTTPGetAsync()
{
// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
//http_client client(U("https://api.mongolab.com/api/1/databases?apiKey=BEHYGwcCIeTysuVyyr6yhmrJDTV3W16L"));
http_client client(U("https://api.twitch.tv/kraken/streams/bananaslamjamma"));
uri_builder builder;
// Append the query parameters: ?method=flickr.test.echo&name=value
//builder.append_query(U("method"), U("flickr.test.echo"));
//builder.append_query(U("name"), U("value"));

auto path_query_fragment = builder.to_string();

// Make an HTTP GET request and asynchronously process the response
return client.request(methods::GET, path_query_fragment).then([](http_response response)
{
// Display the status code that the server returned
std::wostringstream stream;
stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
std::wcout << stream.str();

stream.str(std::wstring());
stream << L"Content type: " << response.headers().content_type() << std::endl;
stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
std::wcout << stream.str();

auto bodyStream = response.body();
streams::stringstreambuf sbuffer;
auto& target = sbuffer.collection();
bodyStream.read_to_end(sbuffer).get();
bodyStream.
stream.str(std::wstring());
stream << L"Response body: " << target.c_str();
std::wcout << stream.str();
});
}
*/
int wmain(int argc, wchar_t *args[])
{
	/*std::wcout << L"Calling HTTPGetAsync..." << std::endl;
	// In this case, I call wait. However, you won’t usually want to wait for the asynchronous operations
	HTTPGetAsync().wait();
	system("pause");
	return 0;*/
	
	for (;;)
	{


	wcout << L"Calling RequestJSONValueAsync..." << endl;
	clock();
	RequestJSONValueAsync().wait();
	for (int w = 0; w < streamList.size(); w++)
	{
	replace(streamList[w], "{width}", "1920");
	replace(streamList[w], "{height}", "1080");
	VideoCapture cap = VideoCapture(streamList[w]);
	Mat image;
	if (cap.isOpened()) {
	cap.read(image);
	}
	string write = "..\\img\\image";
	write += std::to_string(timer);
	write += std::to_string(w);
	write += ".png";
	imwrite(write, image);
	}
	timer++;
	std::this_thread::sleep_for(std::chrono::minutes(10));
	}
	
	//Mat morph;
	//int morph_elem = 0;
	//int morph_size = 0;
	//int morph_operator = 0;
	//int const max_operator = 4;
	//int const max_elem = 2;
	//int const max_kernel_size = 21;
	//int operation = 2;
	//int thresh = 200;
	//int max_thresh = 255;
	//Mat result, src_gray, canny_output;
	//RNG rng(12345);
	////apply greyscale and binary threshold on base image with inputs of 190,255 rgb
	//Mat img = imread("..\\img\\image10451.png", 1);
	//cvtColor(img, img, CV_BGR2GRAY);
	//blur(img, src_gray, Size(3, 3));
	//threshold(img, result, 120, 255, THRESH_BINARY);
	//imwrite("..\\img\\threshold.png", result);
	//Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	//morphologyEx(result, morph, operation, element);

	////resultant morph. transformation image
	//imwrite("..\\img\\morph.png", morph);
	////Find all the contours in the morph. image
	//vector <vector<Point>> contours;
	//vector<Vec4i> hierarchy;
	//vector <vector<Point>> refinedContourList;
	//vector <vector<Point>> refinedTop;
	//vector <vector<Point>> refinedHigh;
	//vector <vector<Point>> refinedBottom;
	//vector <vector<Point>> refinedLow;
	//vector <vector<Point>> refinedLeft;
	//vector <vector<Point>> refinedRight;
	//findContours(result, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	////find moments and centroids in all the contours and store it in array mu (moment) and mc (moment centroid)
	//vector<Moments> mu(contours.size());
	//for (int i = 0; i < contours.size(); i++){
	//	mu[i] = moments(contours[i], false);
	//}

	//vector<Point2f> mc(contours.size());
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	if (contourArea(contours[i]) > 0){
	//		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	//	}

	//}

	////*********** DRAWING IMAGE IS FOR DEBUGGING ONLY**********************//

	////draws all contours in an image for debuging purposes. 
	////note we only allow images of contour area > 30 and arclength < 1000. therefore this will only result in only diamond contours and not any edge contours
	//Mat drawing = Mat::zeros(result.size(), CV_8UC3);
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	if (contourArea(contours[i]) > 200 && arcLength(contours[i], true) < 1000){
	//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//		mu[i] = moments(contours[i], false);
	//		refinedContourList.push_back(contours[i]); //note refinedContourList has all the relevant contours that we require for case 1
	//		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	//		circle(drawing, mc[i], 4, color, -1, 8, 0);
	//	}

	//}
	////***********DRAWING IMAGE IS FOR DEBUGGING ONLY**********************//

	////resultant image after drawing all contours with some contourArea and ContourArcLength filters applied
	//imwrite("..\\img\\finale.png", drawing);
	return 0;
}