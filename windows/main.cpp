#include "window.h"

int main(int argc, char** argv) {
    const char* filename = "obj/test.obj";
    Model *model = new Model(filename);
    //model->load_obj("obj/kgirl/face.obj");
    //model->load_obj("obj/kgirl/hair.obj");
    //model->load_obj("obj/kgirl/pupils.obj");
    model->load_texture(filename, "_diffuse.tga", "diffuse");
    model->load_texture(filename, "_nm.tga", "normal");
    model->load_texture(filename, "_spec.tga", "specular");
    //Skeleton *skeleton = new Skeleton("./obj/kgirl/kgirl.ani");
    // TGAImage *dep_img = new TGAImage(800, 800, TGAImage::RGB);
    TGAImage* img = new TGAImage(800, 800, TGAImage::RGB);
    Point3f pos(0, 0, 3), look(0, 0, 0), light_src(-1, 1, -1);
    Vector3f up(0, 1, 0);
    Light *light = new PointLight(light_src);
    std::vector<Light*> all_light; all_light.push_back(light);
    deffered_shading_phong shader(all_light);
    // int w = dep_img->get_width(), h = dep_img->get_height();
    // Cameral dep_cameral(*dep_img, light_src, look, up, 1, 100, -5, 5, -5, 5);
    // Transform cameral2screen = Scale(w / 2, h / 2, 1) * Translate(Vector3f(1, 1, 1)) *
    //     dep_cameral.Cameral2NDC();
    Cameral cameral(*img, pos, look, up, 1, 10, -0.75, 0.75, -0.75, 0.75);
    // iShader *depth = new Depth();
    // framebuffer *dep_buffer = new framebuffer(800, 800, 4);
    framebuffer *buffer = new framebuffer(800, 800, 4);
    //iShader *shader= new Animation(light, skeleton);
    //iShader *shader = new Phong(light, pos);
    //iShader* shader = new Shadow(light, dep_buffer->depth_buffer, w, h, pos, cameral2screen * dep_cameral.World2Cameral());
    platform_initialize();
    Window *window;
    window = window_create("3D Viewer", 800, 800);
    State *state = new State();
    window_set_userdata(window, state) ;
    int tick = 0;
    TGAImage gbuffer_diffuse(800, 800, TGAImage::RGB);
    Vector3f *gbuffer_normal = new Vector3f[800 * 800];
    Point3f* gbuffer_pos = new Point3f[800 * 800];
    //float min_time = skeleton->min_time, max_time = skeleton->max_time;
    //float ctime = min_time, delta_time = 0.01;
    while (!window_should_close(window)) {
        tick++;
        /*ctime += delta_time;
        if (ctime > max_time) {
            ctime = min_time;
            skeleton->clear();
        }*/
        //update(ctime, 0, *skeleton);
        std::cout << "iter begin " << tick << "\n";
        //cameral_update(window, cameral);
        cameral_update(window, cameral);
        //cameral.update_pos(Vector2f(0.15, 0));
        //dep_buffer->clear();
        buffer->clear();
        //dep_cameral.Raterize(*model, *depth, dep_buffer->depth_buffer);
        cameral.Deffered(*model, gbuffer_pos, gbuffer_normal, gbuffer_diffuse, buffer->depth_buffer, shader);
        cameral.copy_img(buffer->color_buffer);
        window_draw_buffer(window, buffer);
        state->dolly = 0;
        cameral.clear_img();
        memset(gbuffer_pos, 0, sizeof(Vector3f) * 800 * 800);
        memset(gbuffer_normal, 0, sizeof(Point3f) * 800 * 800);
        gbuffer_diffuse.clear();
        input_poll_events();
    }
    window_destroy(window);
    delete model;
    delete img;
    // delete depth;
    // delete shadow;
    delete buffer;
    //delete dep_buffer;
    delete state;
    platform_terminate();
}