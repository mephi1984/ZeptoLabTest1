#include "RenderSystem.h"
#include "Environment.h"
#include "Utils.h"
#include "Inventory.h"
#include <GL/gl.h>



namespace ZL {

void RenderSystem::initialize() {
    /*
    renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultHideCam", "./defaultHideCam.vertex", "./defaultHideCam.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
    renderer.InitOpenGL();*/
}

void RenderSystem::drawScene(GameObjectManager& gameObjects) {
    static const std::string defaultShaderName = "default";
    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    static const std::string textureUniformName = "Texture";

    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glViewport(0, 0, Environment::width, Environment::height);

    if (Environment::gameIsLoading)
    {
        drawLoadingScreen(gameObjects);
    }
    else
    {
        drawWorld(gameObjects);
        drawUI(gameObjects);
    }
    CheckGlError();
}

void RenderSystem::drawViola(GameObjectManager& gameObjects)
{
    static const std::string defaultShaderName = "default";
    static const std::string colorShaderName = "defaultColor";

    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    static const std::string vColorName = "vColor";
    static const std::string textureUniformName = "Texture";

    renderer.shaderManager.PushShader(colorShaderName);
    
    renderer.EnableVertexAttribArray(vPositionName);
    renderer.PushPerspectiveProjectionMatrix(1.0 / 1.5,
        static_cast<float>(Environment::width) / static_cast<float>(Environment::height),
        50, 10000);
    renderer.PushMatrix();

    renderer.LoadIdentity();
    renderer.TranslateMatrix({ 0,0, -100 * Environment::zoom });

    renderer.RotateMatrix(QuatFromRotateAroundX(Environment::cameraAlpha));

    //Go a little bit up to make camera at the position of Viola 
    renderer.TranslateMatrix({ 0, Environment::cameraDefaultVerticalShift, 0 });


    //Viola stuff
    renderer.ScaleMatrix(10);
    renderer.RotateMatrix(QuatFromRotateAroundX(-M_PI / 2.0));
    renderer.RotateMatrix(QuatFromRotateAroundZ(M_PI));


    

    if (Environment::violaCurrentAnimation == 0)
    {
        gameObjects.violaIdleModelMutable.AssignFrom(gameObjects.violaIdleModel.mesh);
        gameObjects.violaIdleModelMutable.RefreshVBO();
        renderer.DrawVertexRenderStruct(gameObjects.violaIdleModelMutable);
    }
    else
    {
        gameObjects.violaWalkModelMutable.AssignFrom(gameObjects.violaWalkModel.mesh);
        gameObjects.violaWalkModelMutable.RefreshVBO();
        renderer.DrawVertexRenderStruct(gameObjects.violaWalkModelMutable);
    }
    
    
    renderer.PopMatrix();
    renderer.PopProjectionMatrix();
    renderer.DisableVertexAttribArray(vPositionName);

    renderer.shaderManager.PopShader();
}

void RenderSystem::drawWorld(GameObjectManager& gameObjects) {
    static const std::string defaultShaderName = "default";
    static const std::string colorShaderName = "defaultColor";
    static const std::string hideCamShaderName = "defaultHideCam";

    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    static const std::string vColorName = "vColor";
    static const std::string textureUniformName = "Texture";

    //static const std::string modelViewMatrixName = "modelView";

    /*
    renderer.shaderManager.PushShader(defaultShaderName);
    renderer.RenderUniform1i(textureUniformName, 0);

    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);
    */



    // Draw cone
    //glBindTexture(GL_TEXTURE_2D, gameObjects.coneTexturePtr->getTexID());
    //renderer.DrawVertexRenderStruct(gameObjects.coneMeshMutable);

    //drawMonster(gameObjects);
    //glClear(GL_DEPTH_BUFFER_BIT);
    drawViola(gameObjects);

    
    renderer.shaderManager.PushShader(hideCamShaderName);
    renderer.RenderUniform1i(textureUniformName, 0);

    Vector3f totalCameraTargetPos = Environment::characterPos - Vector3f{ 0, Environment::cameraDefaultVerticalShift, 0 };

    renderer.RenderUniform3fv("targetPos", &totalCameraTargetPos.v[0]);

    Vector3f cameraPos = Vector3f{ 0,0, 100 * Environment::zoom };

    cameraPos = MultVectorMatrix(cameraPos, QuatToMatrix(QuatFromRotateAroundX(Environment::cameraAlpha)));
    cameraPos = MultVectorMatrix(cameraPos, QuatToMatrix(QuatFromRotateAroundY(Environment::cameraPhi)));

    cameraPos = cameraPos + totalCameraTargetPos;
    renderer.RenderUniform3fv("eyePos", &cameraPos.v[0]);
    //renderer.RenderUniform3fv("eyePos", &testVec2.v[0]);



    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);


    renderer.PushPerspectiveProjectionMatrix(1.0 / 1.5,
        static_cast<float>(Environment::width) / static_cast<float>(Environment::height),
        50, 10000);
    renderer.PushMatrix();

    renderer.LoadIdentity();
    renderer.TranslateMatrix({ 0,0, -100 * Environment::zoom });

    renderer.RotateMatrix(QuatFromRotateAroundX(Environment::cameraAlpha));
    renderer.RotateMatrix(QuatFromRotateAroundY(Environment::cameraPhi));

    renderer.TranslateMatrix(Environment::cameraShift);

    //Go a little bit up to make camera at the position of Viola 
    renderer.TranslateMatrix({ 0, Environment::cameraDefaultVerticalShift, 0 });

    // Draw active objects
    drawObjects(gameObjects);

    // Draw room
    glBindTexture(GL_TEXTURE_2D, gameObjects.rooms[gameObjects.current_room_index].roomTexture->getTexID());
    renderer.DrawVertexRenderStruct(gameObjects.rooms[gameObjects.current_room_index].textMeshMutable);

    if (gameObjects.current_room_index == 1)
    {
        drawMonster(gameObjects);
    }
    drawViola(gameObjects);


    Matrix4f latestProjectionModelView = renderer.GetProjectionModelViewMatrix();
    
    // Проверяем пересечение с мышью после расчета всех матриц
    const_cast<GameObjectManager&>(gameObjects).checkMouseIntersection(
        lastMouseX, lastMouseY, latestProjectionModelView);

    renderer.PopMatrix();
    renderer.PopProjectionMatrix();

    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);
    renderer.shaderManager.PopShader();
    
    // Store matrix for UI rendering
    currentProjectionModelView = latestProjectionModelView;
}

void RenderSystem::drawUI(const GameObjectManager& gameObjects) {
    // Устанавливаем нужный шейдер для UI (например, "default")
    renderer.shaderManager.PushShader("default");

    // Если шейдер ожидает атрибуты вершин, их нужно включить
    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);

    renderer.PushProjectionMatrix(static_cast<float>(Environment::width),
        static_cast<float>(Environment::height));
    renderer.PushMatrix();
    renderer.LoadIdentity();

    // Отрисовка диалогового окна, если оно активно
    if (gameObjects.isDialogActive && gameObjects.dialogTexturePtr) {
        renderer.PushMatrix();
        float xPos = Environment::width / 2.0f - 250;  // Центрируем
        float yPos = Environment::height / 2.0f - 125; // Центрируем
        renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
        renderer.ScaleMatrix(Vector3f{1.5f, 1.5f, 1.0f}); // Увеличиваем размер
        glBindTexture(GL_TEXTURE_2D, gameObjects.dialogTexturePtr->getTexID());
        renderer.DrawVertexRenderStruct(gameObjects.inventoryIconMeshMutable); // Используем 2D меш инвентаря
        renderer.PopMatrix();
    }

    //for (const auto* ao : gameObjects.aoMgr.findByHighlighted(true)) {
    for (auto& ao : gameObjects.rooms[gameObjects.current_room_index].findByHighlighted(true)) {
      std::cout << ao->name << std::endl;
      std::cout << "Draw" << std::endl;
      if (ao->activeObjectScreenTexturePtr) {
          std::cout << "Found activeObjectScreenTexturePtr" << std::endl;
          int screenX, screenY;

          Vector3f objectPosPlusShift = ao->objectPos + Vector3f{ 0, -Environment::cameraDefaultVerticalShift, 0 };

          worldToScreenCoordinates(objectPosPlusShift, currentProjectionModelView,
                                   Environment::width, Environment::height, screenX, screenY);
          renderer.PushMatrix();
          // Здесь можно использовать вычисленные screenX, screenY,
          // но для теста оставляем фиксированное значение
          renderer.TranslateMatrix(Vector3f{screenX + 0.f, screenY + 0.f, 0.0f});
          glBindTexture(GL_TEXTURE_2D, ao->activeObjectScreenTexturePtr->getTexID());
          renderer.DrawVertexRenderStruct(ao->activeObjectScreenMeshMutable);
          renderer.PopMatrix();
      }
    }

    const auto& inventoryMap = ZL::ReturnInventory();
    int i = 0;
    for (const auto& [name, item] : inventoryMap) {
        renderer.PushMatrix();

        if (item.isSelected) {
                  float xPos = Environment::width
                   - gameObjects.INVENTORY_MARGIN
                   - gameObjects.INVENTORY_ICON_SIZE;
        float yPos = gameObjects.INVENTORY_MARGIN
                   + i * (gameObjects.INVENTORY_ICON_SIZE
                   + gameObjects.INVENTORY_MARGIN);
        renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
        renderer.ScaleMatrix(Vector3f{1.5f, 1.5f, 1.0f});
        glBindTexture(GL_TEXTURE_2D, item.texture->getTexID());
        }
        else {

        float xPos = Environment::width
                   - gameObjects.INVENTORY_MARGIN
                   - gameObjects.INVENTORY_ICON_SIZE;
        float yPos = gameObjects.INVENTORY_MARGIN
                   + i * (gameObjects.INVENTORY_ICON_SIZE
                   + gameObjects.INVENTORY_MARGIN);
        renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
        glBindTexture(GL_TEXTURE_2D, item.texture->getTexID());
		}


        renderer.DrawVertexRenderStruct(gameObjects.inventoryIconMeshMutable);

        renderer.PopMatrix();
        i++;
    }

    //    Отрисовка кубиков
            if (gameObjects.current_room_index == 0) {
    for (int j = gameObjects.selectedCubes.size() - 1; j >= 0; j--) {
        auto& cube = gameObjects.selectedCubes[j];
        renderer.PushMatrix();

        float xPos = (gameObjects.SELECTED_CUBE_MARGIN + 300.0f)
                   + j * (gameObjects.SELECTED_CUBE_ICON_SIZE + gameObjects.SELECTED_CUBE_MARGIN);
        float yPos = 500.0f;

        renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
        renderer.ScaleMatrix(Vector3f{2.8f, 2.8f, 1.0f});
        glBindTexture(GL_TEXTURE_2D, cube.texture->getTexID());

        renderer.DrawVertexRenderStruct(gameObjects.inventoryIconMeshMutable);
        renderer.PopMatrix();
    }
}

    renderer.PopMatrix();
    renderer.PopProjectionMatrix();

    // Выключаем атрибуты, чтобы сохранить баланс
    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);

    // Снимаем шейдер, тем самым балансируя стек
    renderer.shaderManager.PopShader();
}

void RenderSystem::drawLoadingScreen(const GameObjectManager& gameObjects)
{
    renderer.shaderManager.PushShader("default");

    // Если шейдер ожидает атрибуты вершин, их нужно включить
    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);

    renderer.PushProjectionMatrix(static_cast<float>(Environment::width),
        static_cast<float>(Environment::height));
    renderer.PushMatrix();
    renderer.LoadIdentity();

    glBindTexture(GL_TEXTURE_2D, gameObjects.loadingScreenTexturePtr->getTexID());
    renderer.DrawVertexRenderStruct(gameObjects.loadingScreenMeshMutable);

    renderer.PopMatrix();
    renderer.PopProjectionMatrix();

    // Выключаем атрибуты, чтобы сохранить баланс
    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);

    // Снимаем шейдер, тем самым балансируя стек
    renderer.shaderManager.PopShader();

}

void RenderSystem::drawMonster(const GameObjectManager& gameObjects)
{
    renderer.shaderManager.PushShader("default");

    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);

    renderer.PushProjectionMatrix(static_cast<float>(Environment::width),
        static_cast<float>(Environment::height), -10, 10);
    renderer.PushMatrix();
    renderer.LoadIdentity();


    std::cout << "Found activeObjectScreenTexturePtr" << std::endl;
    int screenX, screenY;

    Vector3f objectPosPlusShift = Vector3f{ -300, 50, -70 };
    worldToScreenCoordinates(objectPosPlusShift, currentProjectionModelView,
        Environment::width, Environment::height, screenX, screenY);
    renderer.PushMatrix();
    // Здесь можно использовать вычисленные screenX, screenY,
    // но для теста оставляем фиксированное значение
    renderer.TranslateMatrix(Vector3f{ screenX + 0.f, screenY + 0.f, 0.0f });

    if (Environment::monsterState == 0)
    {
        glBindTexture(GL_TEXTURE_2D, gameObjects.monsterTexturePtr1->getTexID());
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, gameObjects.monsterTexturePtr2->getTexID());
    }
    renderer.DrawVertexRenderStruct(gameObjects.monsterScreenMeshMutable);
    renderer.PopMatrix();


    renderer.PopMatrix();
    renderer.PopProjectionMatrix();

    // Выключаем атрибуты, чтобы сохранить баланс
    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);

    // Снимаем шейдер, тем самым балансируя стек
    renderer.shaderManager.PopShader();
}

void RenderSystem::worldToScreenCoordinates(Vector3f objectPos,
    Matrix4f projectionModelView,
    int screenWidth, int screenHeight,
    int& screenX, int& screenY) {

    Vector4f inx = { objectPos.v[0], objectPos.v[1], objectPos.v[2], 1.0f};
    Vector4f clipCoords = MultMatrixVector(projectionModelView, inx);

    float ndcX = clipCoords.v[0] / clipCoords.v[3];
    float ndcY = clipCoords.v[1] / clipCoords.v[3];

    screenX = (int)((ndcX + 1.0f) * 0.5f * screenWidth);
    screenY = (int)((1.0f + ndcY) * 0.5f * screenHeight);
}

void RenderSystem::drawObjects(GameObjectManager& gameObjects){
  for (const auto& ao : gameObjects.activeObjects) {
        renderer.PushMatrix();
        renderer.TranslateMatrix(ao.objectPos);
        glBindTexture(GL_TEXTURE_2D, ao.activeObjectTexturePtr->getTexID());
        renderer.DrawVertexRenderStruct(ao.activeObjectMeshMutable);
        renderer.PopMatrix();
    }
}

} // namespace ZL
