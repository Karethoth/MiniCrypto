#include <GL/glew.h>
#include "render_node.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <cmath> // For sin and cos

minicrypto::RenderNode::RenderNode()
  : minicrypto::NodeInfo("OpenGL Render", {}, {200, 200}),
    rotation_angle(0.0f) // Initialize rotation angle
{
  type = NodeType::Render;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(PinKind::Input);

  setup_framebuffer();
  update_dimensions();
}

void minicrypto::RenderNode::setup_framebuffer()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(node_size.x), static_cast<GLsizei>(node_size.y), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(node_size.x), static_cast<GLsizei>(node_size.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

minicrypto::RenderNode::~RenderNode()
{
  cleanup();
}

void minicrypto::RenderNode::cleanup()
{
  if (framebuffer)
  {
    glDeleteFramebuffers(1, &framebuffer);
    framebuffer = 0;
  }
  if (texture)
  {
    glDeleteTextures(1, &texture);
    texture = 0;
  }
  if (depthbuffer)
  {
    glDeleteRenderbuffers(1, &depthbuffer);
    depthbuffer = 0;
  }
}

void minicrypto::RenderNode::update()
{
  ImNodes::BeginNode(get_id());
  ImGui::Text(name.c_str());

  // Render the triangle
  render();

  draw_pins();
  ImNodes::EndNode();
}

void minicrypto::RenderNode::render()
{
  ImVec2 node_pos = ImGui::GetCursorScreenPos();
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, node_size.x, node_size.y); // Set the viewport size to match the texture size

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Update rotation angle
  rotation_angle += 0.01f; // Adjust the speed of rotation as needed

  // Apply rotation transformation
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -1.0f); // Move the triangle into view
  glRotatef(rotation_angle * (180.0f / 3.14159f), 0.0f, 0.0f, 1.0f); // Convert radians to degrees

  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 0.0f, 0.0f); // Red
  glVertex2f(-0.5f, -0.5f);
  glColor3f(0.0f, 1.0f, 0.0f); // Green
  glVertex2f(0.5f, -0.5f);
  glColor3f(0.0f, 0.0f, 1.0f); // Blue
  glVertex2f(0.0f, 0.5f);
  glEnd();

  glPopMatrix();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ImGui::Image(reinterpret_cast<ImTextureID>(texture), ImVec2(node_size.x, node_size.y), ImVec2(0, 1), ImVec2(1, 0));
}
