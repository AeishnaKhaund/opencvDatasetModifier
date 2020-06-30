//created by harshvardhan
#include "imageOverlayAndAugmentor.h"


void imageOverlayAndAugmentor::imageController(std::string& analysisCustom,std::string& analysis,const long  analysisLineNumbers,const long analysisCustomLineNumbers)
{
   cv::Mat customImage,datasetImage,image;

   std::vector<long int> N1;

    for(long i=0;i<analysisLineNumbers;i++)
        N1.push_back(0);
    N1.push_back(0);
    //minimum number of readymade_images per custom image
    long int imgRatio;
    long int num;
    if(analysisLineNumbers%analysisCustomLineNumbers == 0)
    {
        imgRatio = analysisLineNumbers/analysisCustomLineNumbers;
    }
    else
    {
        imgRatio = analysisLineNumbers/analysisCustomLineNumbers +1 ;
    }

    std::cout << imgRatio << std::endl;
    

    //attaching to os processes
    std::random_device rd;
    std::random_device rd2;
    std::random_device rd3;
    std::random_device rd4;
    std::random_device rd5;
    std::random_device rd6;

    std::mt19937 mt(rd());
    std::mt19937 mt2(rd2());
    std::mt19937 mt3(rd3());
    std::mt19937 mt4(rd4());
    std::mt19937 mt5(rd5());
    std::mt19937 mt6(rd6());

    std::uniform_int_distribution<std::mt19937::result_type> x(0,1920);
    std::uniform_int_distribution<std::mt19937::result_type> y(0,1080);
    std::uniform_int_distribution<std::mt19937::result_type> alpha(1,3);
    std::uniform_int_distribution<std::mt19937::result_type> beta(0,100);
    std::uniform_int_distribution<std::mt19937::result_type> n1(0,analysisLineNumbers);
    std::uniform_int_distribution<std::mt19937::result_type> n2(0,analysisCustomLineNumbers);
    std::uniform_int_distribution<std::mt19937::result_type> imageAdjuster(0,10);
    num = 0;
   


    for(int i=1;i<=analysisCustomLineNumbers;i++)
    {
        cv::Mat datasetImage1;
        int xmin,ymin,xmax,ymax;
        std::ifstream csvReaderCustom(analysisCustom);
        std::string wordCustom,lineCustom;
        std::vector<std::string> rowCustom;
        lineCustom.reserve(300);

        for(int j=0;j<i-1;j++)
            std::getline(csvReaderCustom,lineCustom);
        rowCustom.clear();

        std::getline(csvReaderCustom,lineCustom);
        std::stringstream sCustom(lineCustom);
        while(std::getline(sCustom,wordCustom,','))
        {
            rowCustom.push_back(wordCustom);
        }
        std::cout << rowCustom[1] << std::endl;

        customImage = cv::imread(rowCustom[1],cv::IMREAD_COLOR);

      

        //selecting a random analysiscustomlinenumbers
        std::cout << imgRatio << std::endl;

        for(long int k=0;k<imgRatio;k++)
        { 
            std::ifstream csvReader(analysis);
            std::string word, line;
            std::vector<std::string> row;
            line.reserve(300);
            std::cout << num << std::endl;
            while(N1[num] == 1)
            {
                num = n1(mt6);
                std::cout << num << std::endl;
            }
            std::cout << num << std::endl;

            N1[num] = 1;
            for(long int a=0;a<num-1;a++)
                std::getline(csvReader,line);
            row.clear();
            std::getline(csvReader,line);
            std::stringstream s(line);
            while(std::getline(s,word,','))
            {
                row.push_back(word);
            }
            std::cout << row[1] << std::endl;
            datasetImage = cv::imread(row[1],cv::IMREAD_COLOR);
            std::cout << row[1] << std::endl;
            if((int)imageAdjuster(mt4)>=6)
            {
            datasetImage1 = imageBrightnessAndContrastControl(datasetImage,(int)beta(mt4),(int)alpha(mt5));
            }
            else
            {
                datasetImage1 = datasetImage;
            }
            std::cout << "Hellllooo" << std::endl;
            image = imageOverlay(customImage,datasetImage1,(int)x(rd),(int)y(rd2));

            std::string imageName = std::to_string(num) + "customImages_" + std::to_string(i) + ".jpeg";
            cv::imwrite(imageName,image);
          
        }
        //closing csvReader Custom
  
    }
}

cv::Mat imageOverlayAndAugmentor::imageBrightnessAndContrastControl(cv::Mat image, int brightnessRandom,int contrastRandom) 
{
    cv::Mat modifiedImg = cv::Mat::zeros(image.size(),image.type());
    for(int y =0;y<image.rows;y++)
    {
        for(int x=0;x<image.cols;x++)
        {
            for(int c=0;c<image.channels();c++)
            {
                modifiedImg.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(contrastRandom*image.at<cv::Vec3b>(y,x)[c]+brightnessRandom);
            }
        }
    }
    
    return modifiedImg;
}


cv::Mat imageOverlayAndAugmentor::imageOverlay(cv::Mat customImage, cv::Mat datasetImage,int x,int y) 
{
    std::cout << datasetImage.cols << ":" << datasetImage.rows << std::endl;
    if(1920>(int)customImage.cols+x)
    {
        if(1080 > (int)customImage.rows+y)
        {
            x=x;
            y=y;
        }
        else
        {
            x=x;
            y=1080-(int)customImage.rows;
           

            
        }
        
    }
    else
    {
        if(1080 > (int)customImage.rows + y)
        {
            x=1920-(int)customImage.cols;
            y=y;
      

        }
        else
        {
            x=1920-(int)customImage.cols;
            y=1080-(int)customImage.rows;
          

        }
        
    }

    cv::Mat croppedImage;
    croppedImage = datasetImage(cv::Rect(x,y,customImage.cols,customImage.rows));
    customImage.copyTo(croppedImage);
    return datasetImage;
}
