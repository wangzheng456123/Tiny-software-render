#include "window.h"

int main(int argc, char** argv) {
    const char* filename = "obj/test.obj";
    std::unique_ptr<Model> model(new Model(filename));
    //model->load_obj("obj/kgirl/face.obj");
    //model->load_obj("obj/kgirl/hair.obj");
    //model->load_obj("obj/kgirl/pupils.obj");
    model->load_texture(filename, "_diffuse.tga", "diffuse");
    model->load_texture(filename, "_nm.tga", "normal");
    model->load_texture(filename, "_spec.tga", "specular");
    //std::unique_ptr<Skeleton> skeleton(new Skeleton("./obj/kgirl/kgirl.ani");
    //std::unique_ptr<TGAImage> dep_img(new TGAImage(800, 800, TGAImage::RGB));
    std::unique_ptr<TGAImage> img(new TGAImage(800, 800, TGAImage::RGB));
    Point3f pos(0, 0, 3), look(0, 0, 0), light_src(-1, 1, -1);
    Vector3f up(0, 1, 0);
    std::unique_ptr<Light> light(new PointLight(light_src));
    std::vector<Light*> all_light; all_light.push_back(light.get());
    deffered_shading_phong shader(all_light);
    // int w = dep_img->get_width(), h = dep_img->get_height();
    // Cameral dep_cameral(*dep_img, light_src, look, up, 1, 100, -5, 5, -5, 5);
    // Transform cameral2screen = Scale(w / 2, h / 2, 1) * Translate(Vector3f(1, 1, 1)) *
    //     dep_cameral.Cameral2NDC();
    Cameral cameral(*img, pos, look, up, 1, 10, -0.75, 0.75, -0.75, 0.75);
    //std::unique_ptr<iShader> depth(new Depth());
    //std::unique_ptr<framebuffer> dep_buffer(new framebuffer(800, 800, 4));
    std::unique_ptr<framebuffer> buffer(new framebuffer(800, 800, 4));
    //std::unique_ptr<iShader> shader(new Animation(light, skeleton));
    //std::unique_ptr<iShader> shader(new Phong(light, pos));
    //std::unique_ptr<iShader> shader(new Shadow(light, dep_buffer->depth_buffer, w, h, pos, cameral2screen * dep_cameral.World2Cameral()));
    platform_initialize();
    Window *window;
    window = window_create("3D Viewer", 800, 800);
    std::unique_ptr<State> state(new State());
    window_set_userdata(window, state.get()) ;
    int tick = 0;
    std::unique_ptr<TGAImage> gbuffer_diffuse(new TGAImage(800, 800, TGAImage::RGB));
    std::unique_ptr<Vector3f> gbuffer_normal(new Vector3f[800 * 800]);
    std::unique_ptr<Point3f> gbuffer_pos(new Point3f[800 * 800]);
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
        cameral.Deffered(*model, gbuffer_pos.get(), gbuffer_normal.get(), *gbuffer_diffuse, buffer->depth_buffer, shader);
        cameral.copy_img(buffer->color_buffer);
        window_draw_buffer(window, buffer.get());
        state->dolly = 0;
        cameral.clear_img();
        memset(gbuffer_pos.get(), 0, sizeof(Vector3f) * 800 * 800);
        memset(gbuffer_normal.get(), 0, sizeof(Point3f) * 800 * 800);
        (*gbuffer_diffuse).clear();
        input_poll_events();
    }
    window_destroy(window);
    platform_terminate();
}