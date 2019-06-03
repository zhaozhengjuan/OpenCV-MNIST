#include "training.h"

Training::Training()
{

}
//读取图像数据集
Mat Training::readImages(int opt)
{
    int idx = 0;
    ifstream file;
    Mat img;
    if(opt == 0){
        cout << "\n Training...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-images.idx3-ubyte", ios::binary);
    }
    else{
        cout << "\n Test...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-images.idx3-ubyte", ios::binary);
    }
    // check file
    if (!file.is_open()){
        cout << "\n File Not Found!";
        return img;
    }
    /*
     byte 0 - 3 : Magic Number(Not to be used)
     byte 4 - 7 : Total number of images in the dataset
     byte 8 - 11 : rows of each image in the dataset
     byte 12 - 15 : cols of each image in the dataset
    */
    int magic_number = 0;
    int number_of_images = 0;
    int height = 0;
    int width = 0;
    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_images, sizeof(number_of_images));
    number_of_images = reverseDigit(number_of_images);
    file.read((char*)&height, sizeof(height));
    height = reverseDigit(height);
    file.read((char*)&width, sizeof(width));
    width = reverseDigit(width);
    Mat train_images = Mat(number_of_images, height*width, CV_8UC1);
    cout << "\n No. of images:" << number_of_images <<endl;
    Mat digitImg = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < number_of_images; i++) {
        int index = 0;
        for (int r = 0; r<height; ++r) {
           for (int c = 0; c<width; ++c) {
               unsigned char temp = 0;
               file.read((char*)&temp, sizeof(temp));
               index = r*width + c;
               train_images.at<uchar>(i, index) = (int)temp;
               digitImg.at<uchar>(r, c) = (int)temp;
           }
       }
       if (i < 100) {
            imwrite(format("/home/jackey/GitHub/OpenCV-MNIST/data/images/mnist/images/digit_%d.png", i), digitImg);
       }
    }
    train_images.convertTo(train_images, CV_32FC1);
    return train_images;
}
//读取标记数据集
Mat Training::readLabels(int opt)
{
    int idx = 0;
    ifstream file;
    Mat img;
    if(opt == 0){
        cout << "\n Training...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-labels.idx1-ubyte");
    }
    else{
        cout << "\n Test...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-labels.idx1-ubyte");
    }
     // check file
    if (!file.is_open()){
        cout << "\n File Not Found!";
        return img;
    }
    /*
     byte 0 - 3 : Magic Number(Not to be used)
     byte 4 - 7 : Total number of labels in the dataset
    */
    int magic_number = 0;
    int number_of_labels = 0;
    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_labels, sizeof(number_of_labels));
    number_of_labels = reverseDigit(number_of_labels);
    cout << "\n No. of labels:" << number_of_labels << endl;
    Mat labels = Mat(number_of_labels, 1, CV_8UC1);
    for (long int i = 0; i<number_of_labels; ++i){
        unsigned char temp = 0;
        file.read((char*)&temp, sizeof(temp));
        //printf("temp : %d\n ", temp);
        labels.at<uchar>(i, 0) = temp;
    }
    labels.convertTo(labels, CV_32SC1);
    return labels;
}
/*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们
  率
  KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
*/
void Training::knnTrain()
{
    Mat train_images = readImages(0);
    Mat train_labels = readLabels(0);
    printf("\n read mnist train dataset successfully...\n");
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    knn->setDefaultK(5);
    knn->setIsClassifier(true);
    Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
    knn->train(tdata);
    knn->save("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml");
}

void Training::testMnist()
{
    //Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>("D:/vcprojects/images/mnist/knn_knowledge.yml"); // SVM-POLY - 98%
    Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml"); // KNN - 97%
    Mat train_images = readImages(1);
    Mat train_labels = readLabels(1);
    printf("\n read mnist test dataset successfully...\n");
    float total = train_images.rows;
    float correct = 0;
    Rect rect;
    rect.x = 0;
    rect.height = 1;
    rect.width = (28 * 28);
    for(int i = 0; i < total; i++){
        int actual = train_labels.at<int>(i);
        rect.y = i;
        Mat oneImage = train_images(rect);
        //int digit = svm->predict(oneImage);
        Mat result;
        float predicted = knn->predict(oneImage, result);
        int digit = static_cast<int>(predicted);
        if(digit == actual){
            correct++;
        }
    }
    printf("\n recognize rate : %.2f \n", correct / total);
}

int Training::reverseDigit(int num)
{
    int a = ~num;
    return a;
}
