#include <pgr.h>
#include "entityManager.h"
#include "constants.h"
#include "modelLoader.h"
#include "globe.h"
#include "starship.h"
#include "parameters.h"

namespace nechamar {

    entityManager sceneManager;
    /**
     * updates sceneManager and displays scene
     */
    void timerCallback(int) {

        sceneManager.update();

        glutPostRedisplay();
        glutTimerFunc(nechamar::MS_PER_FRAME, timerCallback, 0);
    }

    void displayCallback() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glUseProgram(handler.program);

        sceneManager.draw();

        CHECK_GL_ERROR();
        glutSwapBuffers();

    }

    void reshapeCallback(int newWidth, int newHeight) {
        glViewport(0, 0, newWidth, newHeight);
        handler.windowWidth = newWidth;
        handler.windowHeight = newHeight;
    }

    void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
        if (GLUT_ACTIVE_CTRL & glutGetModifiers()) keyPressed = keyPressed + 'a' - 1;
        if (keyPressed >= 'A' && keyPressed <= 'Z') keyPressed += -'A' + 'a';
        handler.keys[keyPressed] = true;
    }

    void keyboardUpCallback(unsigned char keyReleased, int mouseX, int mouseY) {
        if (GLUT_ACTIVE_CTRL & glutGetModifiers()) keyReleased += +'a' - 1;

        if (keyReleased >= 'A' && keyReleased <= 'Z') keyReleased += -'A' + 'a';

        handler.keys[keyReleased] = false;

        if(keyReleased == keys::tab){
            if (!handler.destroyMode) {
                glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
                handler.destroyMode = true;
            } else {
                glutSetCursor(GLUT_CURSOR_INFO);
                handler.destroyMode = false;
            }
        }
    }

    void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {
        handler.specKeys[specKeyPressed] = true;


    }

    void specialKeyboardUpCallback(int specKeyReleased, int mouseX, int mouseY) {
        handler.specKeys[specKeyReleased] = false;
    }

    void passiveMouseMotionCallback(int mouseX, int mouseY) {
        handler.mouseDx += handler.mouseX - mouseX;
        handler.mouseDy += handler.mouseY - mouseY;
        handler.mouseX = mouseX;
        handler.mouseY = mouseY;
    }

    void mouseMotionCallback(int mouseX, int mouseY) {
        handler.mouseDx += handler.mouseX - mouseX;
        handler.mouseDy += handler.mouseY - mouseY;
        handler.mouseX = mouseX;
        handler.mouseY = mouseY;
    }

    void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY) {

        if (buttonPressed == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN) {
            unsigned char id;
            glReadPixels(mouseX, handler.windowHeight - 1 - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);

            if (id != 0)  {
                if(handler.destroyMode) {
                    sceneManager.setIdOfHitEntity(id);
                }else{
                    sceneManager.setIdOfMovedEntity(id);
                }
            }
        }else if(!handler.destroyMode && buttonPressed == GLUT_LEFT_BUTTON && buttonState == GLUT_UP){
            sceneManager.setIdOfMovedEntity(0);
        }
    }


    void addLights() {
        light cameraLight;
        cameraLight.type = light_type::spotlight;
        cameraLight.position = glm::vec3(100, 0, 0);
        cameraLight.diffuse = glm::vec3(1, 1, 1);
        cameraLight.ambient = glm::vec3(0.1, 0.1, 0.1);
        cameraLight.specular = glm::vec3(1, 1, 1);
        cameraLight.spotExponent = 2;
        cameraLight.spotCosCutOff = cos(10 * M_PI / 180);
        cameraLight.isOn = true;

        sceneManager.addLight(&cameraLight);

        light sun;
        sun.type = light_type::directional;
        sun.position = glm::vec3(1, 0, 0);
        sun.diffuse = glm::vec3(1, 1, 1);
        sun.ambient = glm::vec3(0.1, 0.1, 0.1);
        sun.specular = glm::vec3(1, 1, 1);
        sun.isOn = true;

        sceneManager.addLight(&sun);
    }

    void addSkybox() {
        std::string *s = new std::string[6]{"res/cube_map.tga", "res/cube_map.tga", "res/cube_map.tga",
                                            "res/cube_map.tga", "res/cube_map.tga", "res/cube_map.tga"};
        skybox *sb = new skybox(s);
        sceneManager.addSkybox(sb);
        handler.sunTexture = pgr::createTexture("res/sun.tga");
    }

    void initializeApplication() {
        GLuint shaders[] = {
                pgr::createShaderFromFile(GL_VERTEX_SHADER, nechamar::VERTEX_SHADER_PATH),
                pgr::createShaderFromFile(GL_FRAGMENT_SHADER, nechamar::FRAGMENT_SHADER_PATH),
                0,
        };

        GLuint shadersSB[] = {
                pgr::createShaderFromFile(GL_VERTEX_SHADER, nechamar::SKYBOX_VERTEX_SHADER_PATH),
                pgr::createShaderFromFile(GL_FRAGMENT_SHADER, nechamar::SKYBOX_FRAGMENT_SHADER_PATH),
                0,
        };

        GLuint shadersEXP[] = {
                pgr::createShaderFromFile(GL_VERTEX_SHADER, nechamar::EXPLOSION_VERTEX_SHADER_PATH),
                pgr::createShaderFromFile(GL_FRAGMENT_SHADER, nechamar::EXPLOSION_FRAGMENT_SHADER_PATH),
                0
        };
        handler.program = pgr::createProgram(shaders);
        handler.pvmMatrix = glGetUniformLocation(handler.program, "pvmMatrix");
        handler.mMatrix = glGetUniformLocation(handler.program, "mMatrix");
        handler.vMatrix = glGetUniformLocation(handler.program, "vMatrix");
        handler.vmMatrix = glGetUniformLocation(handler.program, "vmMatrix");
        handler.nMatrix = glGetUniformLocation(handler.program, "nMatrix");

        handler.position = glGetAttribLocation(handler.program, "position");
        handler.normal = glGetAttribLocation(handler.program, "normal");
        handler.texCords = glGetAttribLocation(handler.program, "texCoords");
        handler.emission = glGetUniformLocation(handler.program, "material.emission");
        handler.diffuse = glGetUniformLocation(handler.program, "material.diffuse");
        handler.ambient = glGetUniformLocation(handler.program, "material.ambient");
        handler.specular = glGetUniformLocation(handler.program, "material.specular");
        handler.shininess = glGetUniformLocation(handler.program, "material.shininess");

        handler.useEmissionTexture = glGetUniformLocation(handler.program, "material.useEmissionTexture");
        handler.emissionTexture = glGetUniformLocation(handler.program, "material.emissionTexture");
        handler.useAmbientTexture = glGetUniformLocation(handler.program, "material.useAmbientTexture");
        handler.ambientTexture = glGetUniformLocation(handler.program, "material.ambientTexture");
        handler.useDiffuseTexture = glGetUniformLocation(handler.program, "material.useDiffuseTexture");
        handler.diffuseTexture = glGetUniformLocation(handler.program, "material.diffuseTexture");

        handler.skyboxProgram = pgr::createProgram(shadersSB);
        handler.screenCoords = glGetAttribLocation(handler.skyboxProgram, "screenCoord");
        handler.skyboxTexture = glGetUniformLocation(handler.skyboxProgram, "skyboxSampler");
        handler.inversePVMatrix = glGetUniformLocation(handler.skyboxProgram, "inversePVmatrix");
        handler.sunTextureLocation = glGetUniformLocation(handler.skyboxProgram, "sun");
        handler.sunAngle = glGetUniformLocation(handler.skyboxProgram, "sunAngle");
        handler.basicAngle = glGetUniformLocation(handler.skyboxProgram, "basicAngle");

        handler.expProgram = pgr::createProgram(shadersEXP);
        handler.expScreenCords = glGetAttribLocation(handler.expProgram, "screenPosition");
        handler.expTexture = glGetUniformLocation(handler.expProgram, "expTexture");
        handler.expPvMatrix = glGetUniformLocation(handler.expProgram, "pvMatrix");
        handler.expT = glGetUniformLocation(handler.expProgram, "t");

        glutWarpPointer(handler.windowWidth / 2, handler.windowHeight / 2);
        handler.mouseX = handler.windowWidth / 2;
        handler.mouseY = handler.windowHeight / 2;

        handler.keys = new bool[255];
        for (int i = 0; i < 255; ++i) {
            handler.keys[i] = false;
        }

        handler.specKeys = new bool[255];
        for (int i = 0; i < 255; ++i) {
            handler.specKeys[i] = false;
        }

        sceneManager.init();

        nechamar::addEntities(&sceneManager);

        addLights();

        addSkybox();

        glClearColor(0, 0, 0, 1.0f);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        CHECK_GL_ERROR();
    }


    void finalizeApplication() {

        pgr::deleteProgramAndShaders(handler.program);
        pgr::deleteProgramAndShaders(handler.skyboxProgram);
        pgr::deleteProgramAndShaders(handler.expProgram);
        delete [] handler.keys;
        delete [] handler.specKeys;
    }

}

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);

    ilInit();

    glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

    // initial window size
    glutInitWindowSize(nechamar::DEFAULT_WINDOW_WIDTH, nechamar::DEFAULT_WINDOW_HEIGHT);
    int window = glutCreateWindow(nechamar::WINDOW_TITLE);

    glutSetWindow(window);
    glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);


    handler.windowWidth = nechamar::DEFAULT_WINDOW_WIDTH;
    handler.windowHeight = nechamar::DEFAULT_WINDOW_HEIGHT;

    glutDisplayFunc(nechamar::displayCallback);
    // register callback for change of window size
    glutReshapeFunc(nechamar::reshapeCallback);
    // register callbacks for keyboard
    glutKeyboardFunc(nechamar::keyboardCallback);
    glutKeyboardUpFunc(nechamar::keyboardUpCallback);
    glutSpecialFunc(nechamar::specialKeyboardCallback);     // key pressed
    glutSpecialUpFunc(nechamar::specialKeyboardUpCallback); // key released
    glutPassiveMotionFunc(nechamar::passiveMouseMotionCallback);
    glutMotionFunc(nechamar::mouseMotionCallback);
    glutMouseFunc(nechamar::mouseCallback);

    glutTimerFunc(nechamar::MS_PER_FRAME, nechamar::timerCallback, 0);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("pgr init failed, required OpenGL not supported?");

    nechamar::initializeApplication();


    glutCloseFunc(nechamar::finalizeApplication);

    glutMainLoop();

    return 0;
}
