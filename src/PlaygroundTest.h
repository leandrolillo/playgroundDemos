#ifndef _PLAYGROUND_TEST_H_
#define _PLAYGROUND_TEST_H_


#include<Playground.h>
#include <Tests.h>
#include <Math3d.h>

#include <stdio.h>

#include<tests/Math3dTests.h>
#include "../geometry/tests/CollisionTesterTests.h"
#include "../physics/tests/CollisionDetectorTests.h"
#include "../physics/tests/ContactResolverTests.h"
#include "../resources/tests/PathsTests.h"
#include "../resources/tests/ResourceLoadRequestTests.h"
#include "../resources/tests/ResourceLoadResponseTests.h"
#include "../resources/tests/ResourceManagerTests.h"

#include "../video/tests/GeometryResourceAdapterTests.h"
#include "../video/tests/ObjResourceAdapterTests.h"

class PlaygroundTestsRunner: public PlaygroundRunner, UnitTest {
		Logger *logger;
		TestsManager testsManager;
        Math3dTests math3dTests;
		ResourceManagerTests resourceManagerTests;
		ResourceLoadRequestTests resourceLoadRequestTests;
		ResourceLoadResponseTests resourceLoadResponseTests;
        CollisionTesterTests collisionTesterTests;
		CollisionDetectorTests collisionDetectorTests;
		ContactResolverTests collisionResolverTests;
		PathsTests pathsTests;

		GeometryResourceAdapterTests geometryResourceAdapterTests;
		ObjResourceAdapterTests objResourceAdapterTests;

	public:
		static const unsigned char ID = 100;

	public:
		PlaygroundTestsRunner() : testsManager(this) {
		    logger = LoggerFactory::getLogger("PlaygroundTestsRunner");
		}
		unsigned char getId() const override {
			return PlaygroundTestsRunner::ID;
		}

        virtual bool afterInit() override {
            //before();
            //tests
            testsManager.doTests();

            //after();

            return true;
        }

        virtual LoopResult doLoop() override {
            return LoopResult::FINISHED;
        }

		virtual bool init() override {
            this->addTest("testMousePicking", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PlaygroundTestsRunner::testMousePicking));

            this->testsManager.addTest(pathsTests);
            this->testsManager.addTest(resourceManagerTests);
            this->testsManager.addTest(resourceLoadRequestTests);
            this->testsManager.addTest(resourceLoadResponseTests);
            this->testsManager.addTest(math3dTests);
            this->testsManager.addTest(collisionTesterTests);
            this->testsManager.addTest(collisionDetectorTests);
            this->testsManager.addTest(collisionResolverTests);
            this->testsManager.addTest(geometryResourceAdapterTests);
            this->testsManager.addTest(objResourceAdapterTests);
            this->testsManager.addTest(*this);


//			testsManager.addTest("testLoadWav", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadWav));
//			testsManager.addTest("testLoadOgg", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadOgg));
//			testsManager.addTest("testLoadBuffer", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadBuffer));
//			testsManager.addTest("testCreateSource", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testCreateSource));
//			testsManager.addTest("testLoadPng", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadPng));
//			testsManager.addTest("testLoadJpeg", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadJpeg));
//			testsManager.addTest("testLoadTga", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadTga));
//			testsManager.addTest("testLoadTexture", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadTexture));
//			testsManager.addTest("testLoadInvalidResource", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testInvalidResource));
//			testsManager.addTest("testFileParser", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testFileParser));
//			testsManager.addTest("testLoadGeometry", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadGeometry));
//			testsManager.addTest("testLoadVertexArray", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadVertexBuffer));
//			testsManager.addTest("testLoadShader", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadShaders));
//			testsManager.addTest("testLoadShaderProgram", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadShaderProgram));
//			testsManager.addTest("testLoadShaderProgramByVersion", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testLoadShaderProgramByVersion));
//			testsManager.addTest("testMath", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testMath));
			//testsManager.addTest("testMousePicking", static_cast<void (UnitTest::*)()>(&PlaygroundTestsRunner::testMousePicking));

			return true;
		}

  	virtual String toString() const override {
  		return "PlaygroundTestsRunner(id:" + std::to_string(this->getId()) + ")";
  	}


		void testMousePicking(PlaygroundRunner *runner)
        {
		    vector4 point(0, 0, -1, 0);

		    unsigned int width = 640;
		    unsigned int height = 480;

		    Camera camera;
		    camera.setPerspectiveProjectionFov(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0);
		    camera.setViewMatrix(matriz_4x4::identidad);

		    vector rayDirection = camera.getRayDirection(320, 240, width, height);

//		    vector4 eyePoint = camera.getViewMatrix() * point;
//		    vector4 projectedPoint = camera.getProjectionMatrix() * point;
//		    vector2 ndcPoint = (vector2(projectedPoint.x, projectedPoint.y) + vector2(1, 1)) * (real)0.5;// ((vector)projectedPoint) * ((real)1 / projectedPoint.w);
//            vector2 renderedPoint = ndcPoint;
//
//		    renderedPoint.x *= (real)width;
//		    renderedPoint.y *= (real)height;
//
//		    printf("Transformed point point: %s\n->Eye point: %s\n->Projected point: %s\n->NDC point: %s\n->ViewPort point: %s\n",
//		            point.toString("%.2f").c_str(),
//		            eyePoint.toString("%.2f").c_str(),
//		            projectedPoint.toString("%.2f").c_str(),
//		            ndcPoint.toString("%.2f").c_str(),
//		            renderedPoint.toString("%.2f").c_str());

		    assertEquals(defaultAssertMessage, vector(0, 0, -1), rayDirection);
        }
		void testMath()
		{
//		    matriz_mxn matriz_mn;
//            logger->debug("matriz nula: [%s]", matriz_mn.toString().c_str());
//
//            matriz_mn = matriz_mxn::identidad(2);
//            assertMatrixEquals(defaultAssertMessage, matriz_mn, matriz_2x2(1, 0, 0, 1));
//
//            logger->debug("matriz identidad 2x2:\n[%s]", matriz_mn.toString().c_str());
//
//            matriz_mn = matriz_mxn::identidad(3);
//            assertMatrixEquals(defaultAssertMessage, matriz_mn, matriz_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));
//
//            logger->debug("matriz identidad 3x3:\n[%s]", matriz_mn.toString().c_str());
//
//            matriz_2x2 matriz2(1, 2, 3, 4);
//            assertEquals(defaultAssertMessage, (double)matriz2(0, 0), 1.0);
//            assertEquals(defaultAssertMessage, (double)matriz2(0, 1), 2.0);
//            assertEquals(defaultAssertMessage, (double)matriz2(1, 0), 3.0);
//            assertEquals(defaultAssertMessage, (double)matriz2(1, 1), 4.0);
//            logger->debug("matriz2:\n[%s]", matriz2.toString().c_str());
//
//            matriz_mn = matriz_2x2(1, 2, 3, 4);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(0, 0), 1.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(0, 1), 2.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(1, 0), 3.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(1, 1), 4.0);
//
//            logger->debug("matriz:\n[%s]", matriz_mn.toString().c_str());
//
//            matriz_mn = matriz_mn * 2.0;
//            assertEquals(defaultAssertMessage, (double)matriz_mn(0, 0), 2.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(0, 1), 4.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(1, 0), 6.0);
//            assertEquals(defaultAssertMessage, (double)matriz_mn(1, 1), 8.0);
//
//            logger->debug("matriz * 2:\n[%s]", matriz_mn.toString().c_str());
//
//            matriz_2x2 operator1_2x2(1, 2, 3, 4);
//            matriz_2x2 operator2_2x2(1, 2, 3, 4);
//
//            matriz_2x2 result_2x2 = operator1_2x2 * operator2_2x2;
//            logger->debug("2x2 multiplication = %s", result_2x2.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_2x2, matriz_2x2(7, 10, 15, 22));
//
//            result_2x2 = operator1_2x2 + operator2_2x2;
//            logger->debug("2x2 addition = %s", result_2x2.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_2x2, matriz_2x2(2, 4, 6, 8));
//
//            result_2x2 = operator1_2x2 - operator2_2x2;
//            logger->debug("2x2 subtraction = %s", result_2x2.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_2x2, matriz_2x2(0, 0, 0, 0));
//
//            result_2x2 = operator1_2x2 * 3;
//            logger->debug("2x2 scalar multiplication = %s", result_2x2.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_2x2, matriz_2x2(3, 6, 9, 12));
//
//
//            matriz_3x3 operator1_3x3(1, 2, 3, 4, 5, 6, 7, 8, 9);
//            matriz_3x3 operator2_3x3(1, 2, 3, 4, 5, 6, 7, 8, 9);
//            matriz_3x3 result_3x3 = operator1_3x3 * operator2_3x3;
//            logger->debug("3x3 multiplication = %s", result_3x3.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_3x3, matriz_3x3(30, 36, 42, 66, 81, 96, 102, 126, 150));
//
//            matriz_mxn operator1_mxn(2, 3);
//            operator1_mxn(0, 0) = 1; operator1_mxn(0, 1) = 2; operator1_mxn(0, 2) = 3;
//            operator1_mxn(1, 0) = 4; operator1_mxn(1, 1) = 5; operator1_mxn(1, 2) = 6;
//
//            matriz_mxn operator2_mxn(3, 2);
//            operator2_mxn(0, 0) = 1; operator2_mxn(0, 1) = 2;
//            operator2_mxn(1, 0) = 3; operator2_mxn(1, 1) = 4;
//            operator2_mxn(2, 0) = 5; operator2_mxn(2, 1) = 6;
//
//            matriz_mxn result_mxn = operator1_mxn * operator2_mxn;
//            logger->debug("mxn multiplication = %s", result_mxn.toString().c_str());
//            assertMatrixEquals(StringFormatter::format("%s, at line: %d", __FILE__, __LINE__), result_mxn, matriz_2x2(22, 28, 49, 64));
//
//            try {
//                result_mxn = operator1_mxn - operator2_mxn;
//                assertFail(assertMessage("Matrix dimensions don't match - exception expected"));
//            } catch (Exception &e) {
//                ;
//            }
//
//            matriz_mxn expected_2x3(2, 3);
//            expected_2x3(0, 0) = 0; expected_2x3(0, 1) = 0; expected_2x3(0, 2) = 0;
//            expected_2x3(1, 0) = 0; expected_2x3(1, 1) = 0; expected_2x3(1, 2) = 0;
//
//            result_mxn = operator1_mxn - operator1_mxn;
//            logger->debug("mxn substraction = %s", result_mxn.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_mxn, expected_2x3);
//
//            expected_2x3(0, 0) = 2; expected_2x3(0, 1) = 4; expected_2x3(0, 2) = 6;
//            expected_2x3(1, 0) = 8; expected_2x3(1, 1) = 10; expected_2x3(1, 2) = 12;
//
//            result_mxn = operator1_mxn + operator1_mxn;
//            logger->debug("mxn addition = %s", result_mxn.toString().c_str());
//            assertMatrixEquals(defaultAssertMessage, result_mxn, expected_2x3);
		}
		void testInvalidResource()
		{
				Resource *resource = this->getContainer()->getResourceManager()->load("tests/fake.wav");
				assertEquals("Null resource expected", null, resource);

				resource = this->getContainer()->getResourceManager()->load("tests/fake.nomimetype");
				assertEquals("Null resource expected", null, resource);

		}
		void testFileParser()
		{
			String token;

			FileParser commentFileParser(this->getContainer()->getResourceManager()->normalize("tests/commentFileToParse.txt"));
			assertEquals("Unexpected token", FileParser::eof, commentFileParser.peekToken());
			assertEquals("Unexpected token", FileParser::eof, commentFileParser.takeToken());
			commentFileParser.close();

			FileParser emptyFileParser(this->getContainer()->getResourceManager()->normalize("tests/emptyFileToParse.txt"));
			assertEquals("Unexpected token", FileParser::eof, emptyFileParser.peekToken());
			assertEquals("Unexpected token", FileParser::eof, emptyFileParser.takeToken());
			commentFileParser.close();

			FileParser fileParser(this->getContainer()->getResourceManager()->normalize("tests/fileToParse.txt"));
			assertEquals("Unexpected token", "{", fileParser.peekToken());
			assertEquals("Unexpected token", "{", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "property", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", ":", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "value", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "}", fileParser.takeToken());
			assertEquals("Unexpected token", FileParser::eof, fileParser.peekToken());
			assertEquals("Unexpected token", FileParser::eof, fileParser.takeToken());
		}
		void testLoadWav()
		{
			AudioResource *resource = (AudioResource *)this->getContainer()->getResourceManager()->load("tests/audio.wav");
			assertTrue("WAV resource not loaded", resource != null);
			assertTrue("WAV resource data not loaded", !resource->getData().empty());
			assertEquals("WAV mimetype invalid", "audio/wav", resource->getMimeType());
		}

		void testLoadOgg() {
			AudioResource *resource = (AudioResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg");
			assertTrue("OGG resource not loaded", resource != null);
			assertTrue("OGG resource data not loaded", !resource->getData().empty());
			assertEquals("OGG mimetype invalid", "audio/ogg", resource->getMimeType());
		}

		void testLoadBuffer() {
			BufferResource *resource = (BufferResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", MimeTypes::AUDIOBUFFER);
			assertTrue("BUFFER resource not loaded", resource != null);
			assertTrue("BUFFER resource id not set correctly", resource->getId() != 0);
			assertEquals("Buffer mimetype invalid", MimeTypes::AUDIOBUFFER, resource->getMimeType());
		}

		void testCreateSource()		{
			Source *resource = (Source *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", MimeTypes::AUDIOSOURCE);
			assertTrue("Source resource not loaded", resource != null);
			assertTrue("Source resource id not set correctly", resource->getId() != 0);
			assertEquals("Source mimetype invalid", MimeTypes::AUDIOSOURCE, resource->getMimeType());

		}

		void testLoadPng() 		{
			ImageResource *resource = (ImageResource *)this->getContainer()->getResourceManager()->load("tests/image.png");
			assertTrue("PNG resource not loaded", resource != null);
			assertTrue("PNG data not set properly", resource->getData() != null);
			assertEquals("PNG Invalid width", 512, resource->getAncho());
			assertEquals("PNG Invalid height", 512, resource->getAlto());
			assertEquals("PNG mimetype invalid", "image/png", resource->getMimeType());
		}

		void testLoadJpeg()		{
		    ImageResource *resource = (ImageResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg");

			assertTrue("JPEG resource not loaded", resource != null);
			assertTrue("JPEG data not set properly", resource->getData() != null);
			assertEquals("JPEG Invalid width", 756, resource->getAncho());
			assertEquals("JPEG Invalid height", 512, resource->getAlto());
			assertEquals("JPEG mimetype invalid", "image/jpeg", resource->getMimeType());
		}

		void testLoadTga()		{
		    ImageResource *resource = (ImageResource *)this->getContainer()->getResourceManager()->load("tests/image.tga");

			assertTrue("TGA resource not loaded", resource != null);
			assertTrue("TGA data not set properly", resource->getData() != null);
			assertEquals("TGA Invalid width", 400, resource->getAncho());
			assertEquals("TGA Invalid height", 300, resource->getAlto());
			assertEquals("TGA mimetype invalid", "image/tga", resource->getMimeType());
		}

		void testLoadTexture()		{
			TextureResource *resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.png", MimeTypes::TEXTURE);

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", MimeTypes::TEXTURE, resource->getMimeType());

			resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg", MimeTypes::TEXTURE);

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", MimeTypes::TEXTURE, resource->getMimeType());

			resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.tga", MimeTypes::TEXTURE);

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", MimeTypes::TEXTURE, resource->getMimeType());


		}

		void testLoadGeometry()	{
			GeometryResource *resource = (GeometryResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", MimeTypes::GEOMETRY);

			assertTrue("GEOMETRY resource not loaded", resource != null);
			assertEquals("GEOMETRY mimetype invalid", MimeTypes::GEOMETRY, resource->getMimeType());
			assertEquals("Incorrect number of vertices", 10, resource->getVertices().size());
			assertEquals("Incorrect number of colors", 3, resource->getColors().size());
			assertEquals("Incorrect number of texture coordinates", 3, resource->getTextureCoordinates().size());
			assertEquals("Incorrect number of normals", 3, resource->getNormals().size());


		}

		void testLoadVertexBuffer()	{
			VertexArrayResource *resource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", MimeTypes::VERTEXARRAY);

			assertTrue("VertexArray resource not loaded", resource != null);
			assertEquals("VertexArray mimetype invalid", MimeTypes::VERTEXARRAY, resource->getMimeType());
		}

		void testLoadShaders()	{
			ShaderResource *resource = (ShaderResource *)this->getContainer()->getResourceManager()->load("tests/vertexShader.glsl", MimeTypes::VERTEXSHADER);

			assertTrue("Shader resource not loaded", resource != null);
			assertEquals("Shader mimetype invalid", MimeTypes::VERTEXSHADER, resource->getMimeType());
		}

		void testLoadShaderProgram()	{
			ShaderProgramResource *resource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("tests/shaderProgram.json", MimeTypes::SHADERPROGRAM);

			assertTrue("Shader Program resource not loaded", resource != null);
			assertEquals("Shader Program mimetype invalid", MimeTypes::SHADERPROGRAM, resource->getMimeType());
		}

		void testLoadShaderProgramByVersion()
		{
//			WglRunner *wgl = (WglRunner *) this->getContainer()->getRunner(0);
//
//			ShaderProgramResource *shaderProgramResource = null;
//
//			if(wgl->getMajorVersion() >= 3) {
//				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.140.program.json", MimeTypes::SHADERPROGRAM);
//			} else {
//				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.120.program.json", MimeTypes::SHADERPROGRAM);
//			}
//			assertTrue("Shader Program resource not loaded", shaderProgramResource != null);
//			assertEquals("Shader Program mimetype invalid", MimeTypes::SHADERPROGRAM, shaderProgramResource->getMimeType());
//
//			if(wgl->getMajorVersion() >= 3) {
//				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.140.program.json", MimeTypes::SHADERPROGRAM);
//			} else {
//				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.120.program.json", MimeTypes::SHADERPROGRAM);
//			}
//			assertTrue("Shader Program resource not loaded", shaderProgramResource != null);
//			assertEquals("Shader Program mimetype invalid", MimeTypes::SHADERPROGRAM, shaderProgramResource->getMimeType());

		}
};

class TestsPlayground: public Playground {
public:
    TestsPlayground(const String &resourcesBasePath) :
            Playground(resourcesBasePath) {
    }
    void init() {
        Playground::init();
        this->addRunner(new PlaygroundTestsRunner());
    }
};

#endif
