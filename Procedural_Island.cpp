/* 
Description: Procedural Island Generator - generates an island using given specs 
Usage: <exe> <-s> <integer> / <exe> 
*/  

/*
Enter grid width (minimum 11): 50
Enter grid height (minimum 11): 50
Enter drop-zone x-coordinate (0 - 50): 25
Enter drop-zone y-coordinate (0 - 50): 25
Enter drop-zone radius (minimum 2): 5
Enter number of particles to drop: 2000
Enter max life of particles: 50
Enter value for waterline (40-200): 40
*/

//Including the required header files
#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "termcolor.hpp"

//Function Prototypes 
double randomNum();
int **makeParticleMap(int width, int height, int windowX, int windowY, int radius, int numParticles, int maxLife);
bool moveExists(int **map, int width, int height, int OrigX, int OrigY, int newX, int newY);
int findMax(int **map, int width, int height);
float **normalizeMap(int **map, int width, int height, int maxVal);
char **polishedMap(float **map, int width, int height, int maxVal, int waterLine);
void srand(unsigned int seed);

//Main
int main(int argc, char **argv)
{
    //Declaring Variables 
    int gWidth;
    int gHeight;
    int dropZone_X;
    int dropZone_Y;
    int radiusOfDrop;
    int numOfParticles;
    int maxLife_Particle;
    int waterLine;

    //Creating an Output File 
    std::ofstream outFile;    
    outFile.open("island.txt");

    //Checking for command line arguments
    if(argc == 3)
    {
        if((strcmp(argv[1],"-s")) == 0)
        {
            srand(atoi(argv[2]));
        }
        else 
        {
            srand(time(0));
        }
    }
    
    else 
    {
        srand(time(0));
    }

    std::cout << "Welcome to Tanmay's island generator!" << std::endl;

    //Taking input for Width of the island
    std::cout << "\nEnter grid width (minimum 11): ";
    std::cin >> gWidth;
    //Checking for bounds
    while (gWidth < 11)
    {
        std::cout << "\nEnter grid width (minimum 11): ";
        std::cin >> gWidth;
    }

    //Taking input for Height of the island
    std::cout << "Enter grid height (minimum 11): ";
    std::cin >> gHeight;
    //Checking for bounds
    while (gHeight < 11)
    {
        std::cout << "Enter grid height (minimum 11): ";
        std::cin >> gHeight;
    }

    //Taking input for the drop zone, X coordinate
    std::cout << "Enter drop-zone x-coordinate (0 - " << gWidth << "): ";
    std::cin >> dropZone_X;
    //Checking the bounds
    while (dropZone_X < 0 || dropZone_X >= gWidth)
    {
        std::cout << "Enter drop-zone x-coordinate (0 - " << gWidth << "): ";
        std::cin >> dropZone_X;
    }

    //Taking input for the Drop zone, Y coordinate 
    std::cout << "Enter drop-zone y-coordinate (0 - " << gHeight << "): ";
    std::cin >> dropZone_Y;
    //Checking the bounds
    while (dropZone_Y < 0 || dropZone_Y >= gHeight)
    {
        std::cout << "Enter drop-zone y-coordinate (0 - " << gHeight << "): ";
    }

    //Taking the inputs for drop zone radius 
    std::cout << "Enter drop-zone radius (minimum 2): ";
    std::cin >> radiusOfDrop;
    //Checking the bounds 
    while (radiusOfDrop < 2)
    {
        std::cout << "Enter drop-zone radius (minimum 2): ";
        std::cin >> radiusOfDrop;
    }

    //Taking the inputs for numOfPartocles to drop
    std::cout << "Enter number of particles to drop: ";
    std::cin >> numOfParticles;
    //Checking the bounds
    while(numOfParticles < 0)
    {
        std::cout << "Enter number of particles to drop: ";
        std::cin >> numOfParticles;    
    }

    //Taking inputs for max life of particles
    std::cout << "Enter max life of particles: ";
    std::cin >> maxLife_Particle;
    //Checking the bounds
    while(maxLife_Particle < 0)
    {
        std::cout << "Enter max life of particles: ";
        std::cin >> maxLife_Particle;
    }

    //Taking the inputs for waterline
    std::cout << "Enter value for waterline (40-200): ";
    std::cin >> waterLine;
    //Checking the bounds
    while (waterLine < 40 || waterLine > 200)
    {
        std::cout << "Enter value for waterline (40-200): ";
        std::cin >> waterLine;
    }
    std::cout << "\n";

    /////////////////////////////////////////////////
    //Raw Map

    //Calling the particleMap function
    int **particle_map = makeParticleMap(gWidth, gHeight, dropZone_X, dropZone_Y, radiusOfDrop, numOfParticles, maxLife_Particle);

    std::cout << "Raw Island: " << std::endl;
    outFile << "Raw Island: " << std::endl;
    
    //Printing the Raw Island
    for (int i = 0; i < gHeight; i++)
    {
        for (int j = 0; j < gWidth; j++)
        {
            std::cout << std::setw(4) << particle_map[i][j];
            outFile << std::setw(4) << particle_map[i][j];
        }//end of for loop
        std::cout << "\n";
        outFile << "\n";
    }//end of for loop

    ////////////////////////////////////////
    ///Max Value

    //Calling the findMax function
    int m = findMax(particle_map, gWidth, gHeight);

    std::cout << "\n";
    outFile << "\n";

    ///////////////////////////////////////////////////
    ///Normalized Map

    //Calling the normalized Map function
    float **normalizedMap = normalizeMap(particle_map, gWidth, gHeight, m);
    std::cout << "Normalized: " << std::endl;
    outFile << "Normalized: " << std::endl;

    //Printing the Normalized island
    for (int i = 0; i < gHeight; i++)
    {
        for (int j = 0; j < gWidth; j++)
        {
            std::cout  << std::setw(4) << normalizedMap[i][j];
            outFile  << std::setw(4) << normalizedMap[i][j];
        }//end of for loop
        std::cout << "\n";
        outFile << "\n";
    }//end of for loop

    std::cout << "\n";
    outFile << "\n";
    //////////////////////////////////////////////////////
    ////Polished Map

    //Calling the polishedMap function
    char **charMap = polishedMap(normalizedMap, gWidth, gHeight, m, waterLine);
    std::cout << "Final: " << std::endl;
    outFile << "Final: " << std::endl;

    //Printing the polished Island
    for (int i = 0; i < gHeight; i++)
    {
        for (int j = 0; j < gWidth; j++)
        {
            //Colouring the characters using termcolor
            if(charMap[i][j] == '#')
            {
                std::cout  << std::setw(3) << termcolor::blue << charMap[i][j];
            }
            else if(charMap[i][j] == '~')
            {
                std::cout  << std::setw(3) << termcolor::cyan << charMap[i][j];
            }
            else if(charMap[i][j] == '.')
            {
                std::cout  << std::setw(3) << termcolor::yellow << charMap[i][j];
            }
            else if(charMap[i][j] == '-')
            {
                std::cout  << std::setw(3) << termcolor::green << charMap[i][j];
            }
            else if(charMap[i][j] == '*')
            {
                std::cout  << std::setw(3) << termcolor::grey << charMap[i][j];
            }
            else if(charMap[i][j] == '^')
            {
                std::cout  << std::setw(3) << termcolor::red << charMap[i][j];
            }
            outFile  << std::setw(3) << charMap[i][j];
        }//end of for loop
        std::cout << "\n";
        outFile << "\n";
    }//end of for loop

    //Closing the file
    outFile.close();

    //Deleting the Array
    for(int i = 0; i < gHeight; i++)
    {
        delete[] particle_map[i];
        delete[] normalizedMap[i];
        delete[] charMap[i];
    }

    delete[] particle_map;
    delete[] normalizedMap;
    delete[] charMap;

    return 0;
} //end of Main()

//RandomNum Function - Generating the random Numbers 
double randomNum()
{
    double temp = (double)rand() / ((double)RAND_MAX + 1);
    return temp;
} //end of randomNum()

//makeParticleMap function - Makes a dynamic 2d Array and Drops the particles 
int **makeParticleMap(int width, int height, int windowX, int windowY, int radius, int numParticles, int maxLife)
{
    //Creating a dynamic 2D Array
    int **intArray;

    intArray = new int *[height];

    for (int i = 0; i < height; i++)
    {
        intArray[i] = new int[width];
    }

    //Initializing the Array with 0 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            intArray[i][j] = 0;
        }
    }//end of for loop

    //Creating variable to store maxLife
    int tempMaxLife;
    tempMaxLife = maxLife;

    //Drop Zone Calculation
    double r;
    double theta;
    double PI = 3.141592;
    int x;
    int y;

    //While loop iterates till the NumOfParticle reaches 0
    while (0 < numParticles)
    {
        maxLife = tempMaxLife;

        //Dropping particles in given radius 
        do
        {
            r = radius * sqrt(randomNum());
            theta = randomNum() * 2 * PI;
            x = (int)(windowX + r * cos(theta));
            y = (int)(windowY + r * sin(theta));
        } while ((x >= width) || (x < 0) || (y >= height) || (y < 0)); // end of do While loop

        //Incrementing the position by 1 if it's a valid move
        intArray[y][x] += 1;

        //While loop iterated to the maxLife reaches 0
        while (0 < maxLife)
        {
            //Creating array to store possible outcomes
            int nX[] = {0, 1, -1};
            int nY[] = {0, 1, -1};

            //Picking up a location randomly to move
            int rNumX = x + nX[rand() % 3];
            int rNumY = y + nY[rand() % 3];

            bool check;
            //Passing that to moveExist to check if the selected location is valid to move
            check = moveExists(intArray, width, height, x, y, rNumX, rNumY);

            //If the location is valid, it increments the value and changes the current coordinates, and reduces maxLife
            if (check == true)
            {
                x = rNumX;
                y = rNumY;
                intArray[y][x] += 1;
                maxLife -= 1;
            }//end of if
        } //end of while Loop
        //Decreasing num of Particles
        numParticles -= 1;
    }//end of while Loop

    return intArray;
}//end of makeParticleMap() function

//moveExist() function - checks if the location generated is valid
bool moveExists(int **map, int width, int height, int OrigX, int OrigY, int newX, int newY)
{
    //Creating a bool variable 
    bool flag = true;
    
    //If the location is valid it returns true
    if ((newX < width) && (newX >= 0) && (newY < height) && (newY >= 0) && (map[newY][newX] <= map[OrigY][OrigX]))
    {
        flag = true;
    }//end of if

    //Else it returns false
    else
    {
        flag = false;
    }//end of else

    return flag;
}//end of moveExist() function

//findMax() function - finds the maximum number in the 2D array
int findMax(int **map, int width, int height)
{
    int maxNum = 0;

    //For loop to iterate through the 2D-Array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] > maxNum)
            {
                maxNum = map[i][j];
            }
        }
    }//end of for loop

    return maxNum;
}//end of findMax() function

//normalizeMap() function - divides every value of the map by maxVal and multiplies by 255
float **normalizeMap(int **map, int width, int height, int maxVal)
{
    //Creating a Dynamic 2D array 
    float **fMap;

    fMap = new float *[height];

    for (int i = 0; i < height; i++)
    {
        fMap[i] = new float[width];
    }

    //Dividing and Multiplying using for loop
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fMap[i][j] = round(((float) map[i][j] / (float) maxVal) * (float) 255);
        }
    }//end of for loop

    return fMap;
}//end of normalizeMap() function

//polishedMap() function - converts the float array to char Array to polish the island
char **polishedMap(float **map, int width, int height, int maxVal, int waterLine)
{
    //Creating a dynamic 2D array
    char **charArray;
    charArray = new char *[height];

    for (int i = 0; i < height; i++)
    {
        charArray[i] = new char[width];
    }

    //Calculating landZone
    int landZone = 255 - waterLine;

    //Converting to CharArray
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(map[i][j] <= (waterLine * 0.5))
            {
                charArray[i][j] = '#';
            }
            else if((map[i][j] > (waterLine * 0.5)) && (map[i][j] <= waterLine))
            {
                charArray[i][j] = '~';
            }
            else if((map[i][j] > waterLine) && (map[i][j] < (waterLine + (0.15 * landZone))))
            {
                charArray[i][j] = '.';
            }
            else if((map[i][j] > waterLine) && (map[i][j] >= (waterLine + (0.15 * landZone))) && (map[i][j] < (waterLine + (0.4 * landZone))))
            {
                charArray[i][j] = '-';
            }
            else if((map[i][j] > waterLine) && (map[i][j] >= (waterLine + (0.4 * landZone))) && (map[i][j] < (waterLine + (0.8 * landZone))))
            {
                charArray[i][j] = '*';
            }
            else 
            {
                charArray[i][j] = '^';
            }
        }
    }//end of for loop

    return charArray;
}//end of polishedMap() function