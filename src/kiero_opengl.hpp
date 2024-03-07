#pragma once

#include <gl/GL.h>
#include <array>

namespace kiero::opengl
{
    static constexpr std::array methodsNames{
        "glAccum", "glAlphaFunc", "glAreTexturesResident", "glArrayElement", "glBegin", "glBindTexture", "glBitmap", "glBlendFunc", "glCallList", "glCallLists", "glClear", "glClearAccum",
        "glClearColor", "glClearDepth", "glClearIndex", "glClearStencil", "glClipPlane", "glColor3b", "glColor3bv", "glColor3d", "glColor3dv", "glColor3f", "glColor3fv", "glColor3i", "glColor3iv",
        "glColor3s", "glColor3sv", "glColor3ub", "glColor3ubv", "glColor3ui", "glColor3uiv", "glColor3us", "glColor3usv", "glColor4b", "glColor4bv", "glColor4d", "glColor4dv", "glColor4f",
        "glColor4fv", "glColor4i", "glColor4iv", "glColor4s", "glColor4sv", "glColor4ub", "glColor4ubv", "glColor4ui", "glColor4uiv", "glColor4us", "glColor4usv", "glColorMask", "glColorMaterial",
        "glColorPointer", "glCopyPixels", "glCopyTexImage1D", "glCopyTexImage2D", "glCopyTexSubImage1D", "glCopyTexSubImage2D", "glCullFaceglCullFace", "glDeleteLists", "glDeleteTextures",
        "glDepthFunc", "glDepthMask", "glDepthRange", "glDisable", "glDisableClientState", "glDrawArrays", "glDrawBuffer", "glDrawElements", "glDrawPixels", "glEdgeFlag", "glEdgeFlagPointer",
        "glEdgeFlagv", "glEnable", "glEnableClientState", "glEnd", "glEndList", "glEvalCoord1d", "glEvalCoord1dv", "glEvalCoord1f", "glEvalCoord1fv", "glEvalCoord2d", "glEvalCoord2dv",
        "glEvalCoord2f", "glEvalCoord2fv", "glEvalMesh1", "glEvalMesh2", "glEvalPoint1", "glEvalPoint2", "glFeedbackBuffer", "glFinish", "glFlush", "glFogf", "glFogfv", "glFogi", "glFogiv",
        "glFrontFace", "glFrustum", "glGenLists", "glGenTextures", "glGetBooleanv", "glGetClipPlane", "glGetDoublev", "glGetError", "glGetFloatv", "glGetIntegerv", "glGetLightfv", "glGetLightiv",
        "glGetMapdv", "glGetMapfv", "glGetMapiv", "glGetMaterialfv", "glGetMaterialiv", "glGetPixelMapfv", "glGetPixelMapuiv", "glGetPixelMapusv", "glGetPointerv", "glGetPolygonStipple",
        "glGetString", "glGetTexEnvfv", "glGetTexEnviv", "glGetTexGendv", "glGetTexGenfv", "glGetTexGeniv", "glGetTexImage", "glGetTexLevelParameterfv", "glGetTexLevelParameteriv",
        "glGetTexParameterfv", "glGetTexParameteriv", "glHint", "glIndexMask", "glIndexPointer", "glIndexd", "glIndexdv", "glIndexf", "glIndexfv", "glIndexi", "glIndexiv", "glIndexs", "glIndexsv",
        "glIndexub", "glIndexubv", "glInitNames", "glInterleavedArrays", "glIsEnabled", "glIsList", "glIsTexture", "glLightModelf", "glLightModelfv", "glLightModeli", "glLightModeliv", "glLightf",
        "glLightfv", "glLighti", "glLightiv", "glLineStipple", "glLineWidth", "glListBase", "glLoadIdentity", "glLoadMatrixd", "glLoadMatrixf", "glLoadName", "glLogicOp", "glMap1d", "glMap1f",
        "glMap2d", "glMap2f", "glMapGrid1d", "glMapGrid1f", "glMapGrid2d", "glMapGrid2f", "glMaterialf", "glMaterialfv", "glMateriali", "glMaterialiv", "glMatrixMode", "glMultMatrixd",
        "glMultMatrixf", "glNewList", "glNormal3b", "glNormal3bv", "glNormal3d", "glNormal3dv", "glNormal3f", "glNormal3fv", "glNormal3i", "glNormal3iv", "glNormal3s", "glNormal3sv",
        "glNormalPointer", "glOrtho", "glPassThrough", "glPixelMapfv", "glPixelMapuiv", "glPixelMapusv", "glPixelStoref", "glPixelStorei", "glPixelTransferf", "glPixelTransferi", "glPixelZoom",
        "glPointSize", "glPolygonMode", "glPolygonOffset", "glPolygonStipple", "glPopAttrib", "glPopClientAttrib", "glPopMatrix", "glPopName", "glPrioritizeTextures", "glPushAttrib",
        "glPushClientAttrib", "glPushMatrix", "glPushName", "glRasterPos2d", "glRasterPos2dv", "glRasterPos2f", "glRasterPos2fv", "glRasterPos2i", "glRasterPos2iv", "glRasterPos2s",
        "glRasterPos2sv", "glRasterPos3d", "glRasterPos3dv", "glRasterPos3f", "glRasterPos3fv", "glRasterPos3i", "glRasterPos3iv", "glRasterPos3s", "glRasterPos3sv", "glRasterPos4d",
        "glRasterPos4dv", "glRasterPos4f", "glRasterPos4fv", "glRasterPos4i", "glRasterPos4iv", "glRasterPos4s", "glRasterPos4sv", "glReadBuffer", "glReadPixels", "glRectd", "glRectdv", "glRectf",
        "glRectfv", "glRecti", "glRectiv", "glRects", "glRectsv", "glRenderMode", "glRotated", "glRotatef", "glScaled", "glScalef", "glScissor", "glSelectBuffer", "glShadeModel", "glStencilFunc",
        "glStencilMask", "glStencilOp", "glTexCoord1d", "glTexCoord1dv", "glTexCoord1f", "glTexCoord1fv", "glTexCoord1i", "glTexCoord1iv", "glTexCoord1s", "glTexCoord1sv", "glTexCoord2d",
        "glTexCoord2dv", "glTexCoord2f", "glTexCoord2fv", "glTexCoord2i", "glTexCoord2iv", "glTexCoord2s", "glTexCoord2sv", "glTexCoord3d", "glTexCoord3dv", "glTexCoord3f", "glTexCoord3fv",
        "glTexCoord3i", "glTexCoord3iv", "glTexCoord3s", "glTexCoord3sv", "glTexCoord4d", "glTexCoord4dv", "glTexCoord4f", "glTexCoord4fv", "glTexCoord4i", "glTexCoord4iv", "glTexCoord4s",
        "glTexCoord4sv", "glTexCoordPointer", "glTexEnvf", "glTexEnvfv", "glTexEnvi", "glTexEnviv", "glTexGend", "glTexGendv", "glTexGenf", "glTexGenfv", "glTexGeni", "glTexGeniv", "glTexImage1D",
        "glTexImage2D", "glTexParameterf", "glTexParameterfv", "glTexParameteri", "glTexParameteriv", "glTexSubImage1D", "glTexSubImage2D", "glTranslated", "glTranslatef", "glVertex2d",
        "glVertex2dv", "glVertex2f", "glVertex2fv", "glVertex2i", "glVertex2iv", "glVertex2s", "glVertex2sv", "glVertex3d", "glVertex3dv", "glVertex3f", "glVertex3fv", "glVertex3i", "glVertex3iv",
        "glVertex3s", "glVertex3sv", "glVertex4d", "glVertex4dv", "glVertex4f", "glVertex4fv", "glVertex4i", "glVertex4iv", "glVertex4s", "glVertex4sv", "glVertexPointer", "glViewport"
    };

    static Status init()
    {
        HMODULE libOpenGL32;
        if ((libOpenGL32 = ::GetModuleHandle(KIERO_TEXT("opengl32.dll"))) == nullptr)
        {
            return Status::ModuleNotFoundError;
        }

        g_methodsTable = static_cast<uintptr_t*>(::calloc(methodsNames.size(), sizeof(uintptr_t)));

        for (size_t i = 0; i < methodsNames.size(); i++)
        {
            g_methodsTable[i] = reinterpret_cast<uintptr_t>(::GetProcAddress(libOpenGL32, methodsNames[i]));
        }

        return Status::Success;
    }
}
