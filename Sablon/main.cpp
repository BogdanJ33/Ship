// Autor: Bogdan Davinic
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
//Biblioteka koje omogucuje automatsku promenu parametara broda
#include "TestBed.h"
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define CRES 30
unsigned int compileShader(GLenum type, const char* source); 
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
float progress = 0.0f; 
bool increaseSpeed = false; 



int main(void)
{

    // Inicijalizacija objekta tipa Ship
    Ship ship = getShip();
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window; 
    unsigned int wWidth = 1200;
    unsigned int wHeight = 1200;
    const char wTitle[] = "Brod";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL); 
    if (window == NULL) 
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate(); 
        return 2; 
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) 
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }


    unsigned int unifiedShader = createShader("basic.vert", "basic.frag"); 
    unsigned int woodShader = createShader("wood.vert", "wood.frag");
    unsigned int waveShader = createShader("wave.vert", "wave.frag");
    unsigned int compasShader = createShader("wave.vert", "compas.frag");

    float circle[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    float r = 0.2; //poluprecnik

    circle[0] = 0; //Centar X0
    circle[1] = 0.1; //Centar Y0
    int i;
    for (i = 0; i <= CRES; i++)
    {

        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 180 / CRES)); //Xi
        circle[2 + 2 * i + 1] = 0.1 + r * sin((3.141592 / 180) * (i * 180 / CRES)); //Yi
    }

    float compas[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    

    compas[0] = 0; //Centar X0
    compas[1] = -0.4; //Centar Y0
    
    for (i = 0; i <= CRES; i++)
    {

        compas[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
        compas[2 + 2 * i + 1] = -0.4 + r * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    }

    float line[] = {
        -1.0, -1.0,
        1.0, 1.0
    };
    float line2[] = {
        -1.0, -1.0,
        1.0, 1.0
    };
    float lineLength = 0.2; // Adjust the length of the line as needed
    float rotationAngle = 2.0;

    float oceanRectangle[] = {
    -1.0, 1.0,0.0, 0.0, 1.0,   // Bottom-left
    -1.0, 0.0,0.0, 0.0, 1.0,    // Top-left
    1.0, 1.0, 0.0,0.0, 1.0,    // Bottom-right
    1.0, 0.0, 0.0, 0.0, 1.0,     // Top-right
    };

    float tableVertices[] = {
    -1.0, 0.0, 0.0, 1.0,   // Bottom-left
    -1.0, -1.0, 0.0, 0.0,  // Top-left
    1.0, 0.0, 1.0, 1.0,  // Bottom-right
    1.0, -1.0, 1.0, 0.0,  // Top-right
    };

    float nameVertices[] = {
        -1.0, -0.8, 0.0, 1.0,
        -1.0, -0.9, 0.0, 0.0,
        -0.2, -0.8, 1.0, 1.0,
        -0.2, -0.9, 1.0, 0.0
    };

    float gasVertices[] =
    {
         0.9, -0.1, 0.0, 0.0, 1.0,
        0.9, -0.2,  0.0, 0.0, 1.0,
        0.86, -0.1, 0.0, 0.0, 1.0,
        0.86, -0.2, 0.0, 0.0, 1.0,
        0.82, -0.1, 0.0, 0.0, 1.0,
        0.82, -0.2, 0.0, 0.0, 1.0,
        0.78, -0.1, 0.0, 0.0, 1.0,
        0.78, -0.2, 0.0, 0.0, 1.0,
        0.74, -0.1, 0.0, 0.0, 1.0,
        0.74, -0.2, 0.0, 0.0, 1.0,
       0.70, -0.1, 0.0, 0.0, 1.0,
        0.70, -0.2, 0.0, 0.0, 1.0,
        0.66, -0.1, 0.0, 0.0, 1.0,
        0.66, -0.2, 0.0, 0.0, 1.0,
        0.62, -0.1, 0.0, 0.0, 1.0,
        0.62, -0.2, 0.0, 0.0, 1.0,
        0.58, -0.1, 0.0, 0.0, 1.0,
        0.58, -0.2, 0.0, 0.0, 1.0,
        0.54, -0.1, 0.0, 0.0, 1.0,
        0.54, -0.2, 0.0, 0.0, 1.0,
    };

    
    

  

    float barVertices[] =
    {
        -0.9, -0.1, 1.0, 1.0, 1.0,
        -0.9, -0.2, 1.0, 1.0, 1.0,
        -0.86, -0.1, 1.0, 1.0, 1.0,
        -0.86, -0.2, 1.0, 1.0, 1.0,
        -0.82, -0.1, 1.0, 1.0, 1.0,
        -0.82, -0.2, 1.0, 1.0, 1.0,
        -0.78, -0.1, 1.0, 1.0, 1.0,
        -0.78, -0.2, 1.0, 1.0, 1.0,
        -0.74, -0.1, 1.0, 1.0, 1.0,
        -0.74, -0.2, 1.0, 1.0, 1.0,
        -0.70, -0.1, 1.0, 1.0, 1.0,
        -0.70, -0.2, 1.0, 1.0, 1.0,
        -0.66, -0.1, 1.0, 1.0, 1.0,
        -0.66, -0.2, 1.0, 1.0, 1.0,
        -0.62, -0.1, 1.0, 1.0, 1.0,
        -0.62, -0.2, 1.0, 1.0, 1.0,
        -0.58, -0.1, 1.0, 1.0, 1.0,
        -0.58, -0.2, 1.0, 1.0, 1.0,
        -0.54, -0.1, 1.0, 1.0, 1.0,
        -0.54, -0.2, 1.0, 1.0, 1.0,


    };

    unsigned int VAO[9];
    glGenVertexArrays(9, VAO);

    unsigned int VBO[9];
    glGenBuffers(9, VBO);

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(compas), compas, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glBindVertexArray(VAO[7]); // Use your VAO for the line
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]); // Use your VBO for the line
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[8]); // Use your VAO for the line
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]); // Use your VBO for the line
    glBufferData(GL_ARRAY_BUFFER, sizeof(line2), line2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVertices), barVertices, GL_STATIC_DRAW);
    // Specify the layout of the vertex data for barVertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oceanRectangle), oceanRectangle, GL_STATIC_DRAW);
    // Specify the layout of the vertex data for gasVertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set up VBO for gasVertices
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gasVertices), gasVertices, GL_STATIC_DRAW);
    // Specify the layout of the vertex data for gasVertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, GL_STATIC_DRAW);
    // Specify the layout of the vertex data for gasVertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    // Specify the layout of the vertex data for gasVertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned checkerTexture = loadImageToTexture("res/wood.jpg"); //Ucitavamo teksturu
    
    glBindTexture(GL_TEXTURE_2D, checkerTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// T = V = Y

    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(woodShader, "uTex");
    glUniform1i(uTexLoc, 0);

    unsigned nameTexture = loadImageToTexture("res/ime.png");
    glBindTexture(GL_TEXTURE_2D, nameTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// T = V = Y

    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTex = glGetUniformLocation(woodShader, "uTex");
    glUniform1i(uTex, 0);
    
    startSimulation(&ship);
    float centerX = 0.0;
    

    // Vremenska promenljiva za animaciju boje popunjenog dela progress bara
    float animationTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        if (centerX >= 1 + r) {
            centerX = -1 - r;
        }
        centerX += 0.0002;
        circle[0] = centerX; // Centar X0
        circle[1] = 0.1;     // Centar Y0
        for (int i = 0; i <= CRES; i++) {
            circle[2 + 2 * i] = centerX + r * cos((3.141592 / 180) * (i * 180 / CRES)); // Xi
            circle[2 + 2 * i + 1] = 0.1 + r * sin((3.141592 / 180) * (i * 180 / CRES)); // Yi
        }
       
        glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
        
        
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        ship.setFuelAmount(ship.getFuelAmount() - 0.01);
        float fuelPercentage = ship.getFuelAmount() / ship.getMaxFuelAmount();
        for (int i = 0; i < 20; i=i+2) {
            int index = i;
            
            if (fuelPercentage <= (i) * 0.05f) {
                gasVertices[(19-index) * 5 + 2] = 1.0f;           // R
                gasVertices[(19 - index) * 5 + 3] = 0.0f;  // G
                gasVertices[(19 - index) * 5 + 4] = 0.0f;
                gasVertices[(19-index-1) * 5 + 2] = 1.0f;           // R
                gasVertices[(19-index-1) * 5 + 3] = 0.0f;  // G
                gasVertices[(19-index-1) * 5 + 4] = 0.0f;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(gasVertices), gasVertices);
        
        
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the left rectangle with the updated colors

        glUseProgram(woodShader);
        glUniform1i(glGetUniformLocation(woodShader, "uTex"), 0);
        glBindVertexArray(VAO[2]);

        //glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, checkerTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        
       
        glBindVertexArray(VAO[3]);

        //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, nameTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(unifiedShader);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);


// Render the right rectangle
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);

        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(waveShader);
        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

        glUseProgram(compasShader);
        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));


        glUseProgram(waveShader);
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_LINES, 0, 2);
        rotationAngle += 0.1;

        float angleInRadians = (3.141592 / 180) * rotationAngle;
        float cosValue = cos(angleInRadians);
        float sinValue = sin(angleInRadians);

        line[0] = 0; // Start point X
        line[1] = -0.4; // Start point Y
        line[2] = lineLength * cosValue; // End point X
        line[3] = -0.4 + lineLength * sinValue; // End point Y
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line), line);

        glBindVertexArray(VAO[8]);
        glDrawArrays(GL_LINES, 0, 2);
        

        angleInRadians = ((3.141592 / 180) * (180 + rotationAngle));
        cosValue = cos(angleInRadians);
        sinValue = sin(angleInRadians);

        line2[0] = 0.0; // Start point X
        line2[1] = -0.4; // Start point Y
        line2[2] = lineLength * cosValue; // End point X
        line2[3] = -0.4 + lineLength * sinValue; // End point Y

        glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line2), line2);


        
        

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (increaseSpeed && progress < 1.0f)
        {
            progress += 0.0001f;
        }

        if (progress >= 1.0f)
        {
            ship.speedUp();
            progress = 0.0f;
            increaseSpeed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        animationTime += 0.001f;

        // Calculate a gradient from left to right based on progress
        float gradient = progress;

        for (int j = 0; j < 20; ++j) {
            int index = j;

            float colorR = sin(animationTime + gradient) * 0.5f + 0.5f;
            float colorG = sin(animationTime + 0.3f + gradient) * 0.5f + 0.5f;
            float colorB = sin(animationTime + 0.6f + gradient) * 0.5f + 0.5f;

            // Update only the left side of the rectangle based on progress
            if (index * 5 + 0 < animationTime * 4 * 5) {
                if (animationTime <= 4.6) {
                    barVertices[index * 5 + 2] = colorR;
                    barVertices[index * 5 + 3] = colorG;
                    barVertices[index * 5 + 4] = colorB;

                }
                else {
                    for (int j = 0; j < 20; ++j) {
                        int index = j;
                        barVertices[index * 5 + 2] = 1.0;
                        barVertices[index * 5 + 3] = 1.0;
                        barVertices[index * 5 + 4] = 1.0;
                    }
                    animationTime = 0;
                }
                glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(barVertices), barVertices);
                
            }
        }

    }
    }

    //Prekid simulacije
    endSimulation(&ship);
    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteProgram(unifiedShader);
    glfwTerminate();
    return 0;
}


unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
