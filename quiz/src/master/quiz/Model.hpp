#ifndef HEADER_quiz_Model_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Model_hpp_ALREADY_INCLUDED

#include <quiz/Question.hpp>
#include <quiz/Score.hpp>
#include <quiz/Board.hpp>
#include <gubg/mss.hpp>
#include <vector>

namespace quiz { 

#define unroll(x) x(Idle) x(NewQuestion) x(Thinking) x(Answer) x(Error)
    enum class State
    {
#define x(name) name,
        unroll(x)
#undef x
    };
    inline std::ostream &operator<<(std::ostream &os, State state)
    {
        switch (state)
        {
#define x(name) case State::name: os << #name; break;
            unroll(x)
#undef x
        }
        return os;
    }
#undef unroll

    class Model
    {
    public:
        class Events
        {
        public:
            virtual void me_show_question(const Question *question_ptr) = 0;
            virtual void me_show_answer_team_order(const std::string &team, const std::string &order) = 0;
            virtual void me_play_music(bool b, bool restart) = 0;
            virtual void me_answer_was_correct(bool b) = 0;
            virtual void me_show_error(const std::string &msg) = 0;
            virtual void me_show_score(const Score *score) = 0;
            virtual void me_show_board(const Board *board) = 0;
        };

        Model()
        {
            if (!construct_(ctor_error_) && ctor_error_.empty())
                ctor_error_ = "Unknown construction error for Model";
        }

        void inject_events_receiver(Events *events) {events_ = events;}

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(ctor_error_.empty(), error = ctor_error_);
            MSS(!!events_, error = "Model.events_ not set");

            if (answer_team_.empty() && !answer_order_.empty())
            {
                answer_team_ = answer_order_.substr(0, 1);
                answer_order_ = answer_order_.substr(1);
                MSS(change_state_(State::Answer, sm_error_));
            }

            events_->me_show_answer_team_order(answer_team_, answer_order_);


            events_->me_show_score(state_ == State::Idle ? &score_ : nullptr);

            {
                Board board;
                for (const auto &question: questions_)
                    if (question.group != '?')
                    {
                        auto &category = board.group__categories[question.group];
                        if (category.name.empty())
                            switch (question.group)
                            {
                                case 'a': category.name = "code"; break;
                                case 'b': category.name = "covers"; break;
                                case 'c': category.name = "film"; break;
                                case 'd': category.name = "wiewat"; break;
                                case 'e': category.name = "music"; break;
                                case 'f': category.name = "faceswap"; break;
                                case 'g': category.name = "kids"; break;
                                case 'h': category.name = "droedel"; break;
                            }
                        const auto ix = question.ix;
                        while (category.status.size() <= ix)
                            category.status.push_back('.');
                        category.status[ix] = question.done ? ' ' : '?';
                    }
                events_->me_show_board(state_ == State::Idle ? &board : nullptr);
            }

            MSS_END();
        }

        void process_char(char ch)
        {
            if (!process_char_(ch))
            {
                if (events_)
                    events_->me_show_error(std::string("Error: ")+sm_error_);
            }
        }

    private:
        const Question *get_question_(unsigned int ix) const
        {
            if (ix >= questions_.size())
                return nullptr;
            return &questions_[ix];
        }

        bool process_char_(char ch)
        {
            MSS_BEGIN(bool);

            MSS(!!events_);

            sm_error_.clear();

            if ('0' <= ch && ch <= '9')
                number_ = number_.value_or(0)*10 + (ch-'0');
            else if (ch == 'q')
                MSS(change_state_(State::NewQuestion, sm_error_));
            else if ('a' <= ch && ch <= 'e')
            {
                switch (state_)
                {
                    case State::Idle:
                    case State::Error:
                        return true;
                        break;
                    default:
                        if (answer_order_.find(ch) != std::string::npos)
                            return true;
                        if (!answer_team_.empty() && answer_team_[0] == ch)
                            return true;
                        answer_order_.push_back(ch);
                        break;
                }
            }
            else if (ch == 'x')
            {
                if (!answer_team_.empty())
                {
                    --score_.team__score[answer_team_];
                    events_->me_answer_was_correct(false);
                    MSS(change_state_(State::Thinking, sm_error_));
                }
            }
            else if (ch == 'v')
            {
                if (!answer_team_.empty())
                {
                    ++score_.team__score[answer_team_];
                    if (0 <= question_ix_ && question_ix_ < questions_.size())
                        questions_[question_ix_].done = true;
                    events_->me_answer_was_correct(true);
                    MSS(change_state_(State::Idle, sm_error_));
                }
            }
            else if (ch == 27)
            {
                MSS(change_state_(State::Idle, sm_error_));
                number_.reset();
            }
            else if (ch == ' ')
            {
                if (current_question_)
                {
                    auto &question = current_question_.value();
                    auto it = question.descriptions.begin();
                    if (it != question.descriptions.end())
                    {
                        question.descriptions.erase(it);
                    }
                    events_->me_show_question(&question);
                }
            }
            else if (ch == 'r')
                events_->me_play_music(true, true);

            MSS_END();
        }

        bool construct_(std::string &error)
        {
            MSS_BEGIN(bool);

            if (false)
            {
                score_.team__score["a"] = 0;
                score_.team__score["b"] = 0;
                score_.team__score["c"] = 0;
                score_.team__score["d"] = 0;
            }

            const std::string folder = "quiz/media/question/";
            unsigned int ix = 0;
            auto add_question = [&](Question &question, char group)
            {
                if (group != '?')
                {
                    question.group = group;
                    question.ix = ix++;
                    if (ix <= 10)
                        questions_.push_back(question);
                }
            };

            auto add_textt = [&](char group, const std::string &title, const std::string &descr, auto ftor)
            {
                Question q;
                q.title = title;
                if (!descr.empty())
                    q.descriptions.push_back(descr);
                ftor(q.descriptions);
                add_question(q, group);
            };
            auto add_image = [&](char group, const std::string &title, const std::string &image_fn)
            {
                Question q;
                q.title = title;
                q.image_fn = folder+image_fn;
                add_question(q, group);
            };
            auto add_music = [&](char group, const std::string &title, const std::string &music_fn, float offset = 0.0f, float pitch = 1.0f)
            {
                Question q;
                q.title = title;
                q.music_fn = folder+music_fn;
                q.offset = offset;
                q.pitch = pitch;
                add_question(q, group);
            };

            auto noop = [](std::vector<std::string> &vec){};

            //Kraak de code
            ix = 0;
            add_textt('a', "Wat is hetvolgende cijfer?", "3.141592", noop);
            add_textt('a', "Roepen maar", "Vl dz vrg n mt\nd jst mdklnkrs:\ndrk f n rp ht wrd\n\"spghtt\"", noop);
            add_image('a', "Vertaal vanuit het Azarts", "azart_spaghetti.png");
            add_image('a', "Op welke plaats staat de auto?", "parkeerplaats.jpg");
            add_image('a', "Leg dit begrip uit", "azart_hypotenusa.png");
            add_music('a', "Welke sport horen we hier?", "morse-high_speed_telegraphy.ogg");
            add_music('a', "Welk woord horen we hier?", "morse-winawinterweekend.ogg");
            add_image('a', "Teken hetvolgende symbool", "complete_12345.png");
            add_textt('a', "Doe de vergelijking kloppen", "1000 = \n8 8 8 8 8 8 8 8\nGebruik enkel:\n+ - x : ( ) of niets", noop);
            add_textt('a', "Just do-it", "Maak het vormpje\nin LEGO na", noop);

            add_textt('a', "Just do-it", "Tel tot 10 in het\nNederlands\nFrans\nEngels", noop);
            add_textt('a', "Hoeveel symbolen\nworden erin\nMorse code gebruikt?", "", noop);

            //Covers
            ix = 0;
            add_music('b', "Geef originele artist", "cover-lady_gaga-varia-david_bowie.ogg", 29.2);
            add_music('b', "Geef originele artist", "cover-leo_moracchioli-dance_monkey-tones_and_i.ogg", 60+52);
            add_music('b', "Geef originele artist", "cover-noordkaap-arme_joe-will_tura.ogg");
            add_music('b', "Geef originele artist", "cover-arno_triggerfinger-j_aime_la_vie-sandra_kim.ogg");
            add_music('b', "Geef originele artist", "cover-battledragon-freestyler-bomfunkmcs.ogg", 12.5);
            add_music('b', "Geef originele artist", "cover-christoff-sweet_caroline-neil_diamond.ogg");
            add_music('b', "Geef originele artist", "cover-children_of_bodom-oops_i_did_it_again-britney_spears.ogg");
            add_music('b', "Geef originele artist", "cover-sarah_bettens_jan_de_wilde-de_aarde-urbanus.ogg", 2*60+16);
            add_music('b', "Geef originele artist", "cover-ghost-it_s_a_sin-pet_shop_boys.ogg", 10);
            add_music('b', "Geef originele artist", "cover-arno-les_files_du_bord_de_mer-adamo.ogg");
            add_music('b', "Geef originele artist", "cover-bazart-sterrenstof-jeugdvantegenwoordig.ogg");

            add_music('b', "Geef originele artist", "cover-disturbed-the_sound_of_silence-simon_and_garfunkel.ogg", 12);
            add_music('b', "Geef originele artist", "cover-hallelujah-pentatonix-leonard_cohen.ogg", 5.5);
            add_music('b', "Geef originele artist", "cover-anna_ternheim-when_tomorrow_comes-eurythmics.ogg");
            add_music('b', "Geef originele artist", "cover-leo-moracchioli-ring_of_fire-johnny_cash.ogg", 6.7);
            add_music('b', "Geef originele artist", "cover-metal-last_christmas-wham.ogg", 58.2);
            add_music('b', "Geef originele artist", "cover-sinead_o_connor-nothing_compares_2u-prince.ogg");

            //Film en muziek
            ix = 0;
            add_music('c', "Geef film of serie", "movie-friends.ogg");
            add_music('c', "Geef film of serie", "movie-callbays.ogg", 10.4);
            add_music('c', "Geef film of serie", "movie-star_wars.ogg", 60+35.5);
            add_music('c', "Geef film of serie", "movie-die_hard.ogg", 6.5);
            add_music('c', "Geef film of serie", "movie-king_bob.ogg", 60+53.4);
            add_music('c', "Geef film of serie", "movie-hans_teeuwen.ogg", 60+34.5);
            add_music('c', "Geef film of serie", "movie-jungle_book.ogg");
            add_music('c', "Geef film of serie", "movie-jurassic_park.ogg", 60+33);
            add_music('c', "Geef film of serie", "movie-la_casa_de_papel.ogg");
            add_music('c', "Geef film of serie", "movie-howimetyourmother.ogg", 7.35);

            add_music('c', "Geef film of serie", "movie-mission_impossible.ogg");
            add_music('c', "Geef film of serie", "movie-pippi_langkous.ogg");
            add_music('c', "Geef film of serie", "movie-the_lord_of_the_rings.ogg", 60+43.8);
            add_music('c', "Geef film of serie", "movie-journaal_vrt.ogg");
            add_music('c', "Geef film of serie", "movie-godzilla-puff_daddy-come_with_me.ogg", 34.5);
            add_music('c', "Geef film of serie", "movie-game_of_thrones.ogg");

            //Wie of wat
            ix = 0;
            add_textt('d', "Wat ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ork, ork, ork!");
                    vec.emplace_back("Soep eet je\nmet een ...!");
                    });
            add_textt('d', "Wie ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik ben geboren in\nde middeleeuwen");
                    vec.emplace_back("Ik ben katholiek");
                    vec.emplace_back("Heel veel mensen\nkomen dagelijks\nnaar mij kijken");
                    vec.emplace_back("Victor Hugo heeft\neen verhaaltje over\nmij geschreven");
                    vec.emplace_back("Ik woon op\neen eiland");
                    vec.emplace_back("Vorige lente had\nik last van een brandje");
                    vec.emplace_back("Ik woon in\nParijs");
                    });
            add_textt('d', "Wie ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik woon in\nLommel");
                    vec.emplace_back("Ik had om 18u\nop 2019/07/28\nmoeten starten");
                    vec.emplace_back("De burgemeester\nheeft mij genekt");
                    vec.emplace_back("Ik was niet\nveilig genoeg");
                    vec.emplace_back("Ik ben Fyre\nin \'t klein");
                    vec.emplace_back("Ik ben een\nfestival");
                    vec.emplace_back("Mijn naam\nklinkt zelfs\nals festival");
                    });
            add_textt('d', "Wat ben ik?", "", [](auto &vec){
                    vec.emplace_back("Elk, elk, elk!");
                    vec.emplace_back("Een koe drinkt ...!");
                    });
            add_textt('d', "Wie ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik spreek Antwerps");
                    vec.emplace_back("Ik hou van klinkers");
                    vec.emplace_back("Ik was ook graag\npiloot geweest");
                    vec.emplace_back("Ik was ook graag\nvliegtuig geweest");
                    vec.emplace_back("Nu ben ik niet meer");
                    vec.emplace_back("Mijn werk kan je\nnog wel komen\nbekijken");
                    vec.emplace_back("Mijn echte naam is\nHenri Van Herwegen");
                    });
            add_textt('d', "Wat ben ik?", "", [](auto &vec){
                    vec.emplace_back("A: We willen\neen eenhoorn");
                    vec.emplace_back("B: Eenhoorns bestaan\nniet. Je kan\neen pony krijgen");
                    vec.emplace_back("A: We stemmen\ntegen een pony");
                    vec.emplace_back("B: Dan krijg\nje niets");
                    vec.emplace_back("A: We stemmen\ntegen \"niets\"");
                    vec.emplace_back("B: Wat wil je\ndan wel?");
                    vec.emplace_back("A: We willen\neen eenhoorn");
                    });
            add_textt('d', "Wat ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik lach met iedereen");
                    vec.emplace_back("Niet iedereen\nkan daar mee\nlachen");
                    vec.emplace_back("Zeker niet\nna dit jaar");
                    vec.emplace_back("Ze willen\nme schrappen");
                    vec.emplace_back("We schrappen\nliever ons zelf");
                    vec.emplace_back("Dat zou dan\nwel de eerste\nkeer zijn");
                    vec.emplace_back("\'t Zijn allemaal\nvuil janetten");
                    vec.emplace_back("Ik vind plaats\nin Aalst");
                    });
            add_image('d', "Welke 7 albums ben ik?", "11_albums.jpg");
            add_textt('d', "Wie ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik zeg tegen\niedereen mijn\ngedacht");
                    vec.emplace_back("Niet iedereen\nkan daar mee\nlachen");
                    vec.emplace_back("Ze hebben mijn\njeugd verpest");
                    vec.emplace_back("Soms pak ik\nde boot");
                    vec.emplace_back("Liever geen\nvliegtuig");
                    vec.emplace_back("Ik ben al\neven niet meer\nnaar school geweest");
                    vec.emplace_back("How dare you!");
                    vec.emplace_back("Ik was Time\nPerson of the Year");
                    });
            add_textt('d', "Wie ben ik?", "", [](auto &vec){
                    vec.emplace_back("Ik ben een figuur uit\nStar Wars");
                    vec.emplace_back("Ik ben van de dark\nside");
                    vec.emplace_back("Rey moet opa tegen\nmij zeggen");
                    vec.emplace_back("Ik ben de leermeester\nvan Darth Vader");
                    vec.emplace_back("Ik was keizer van\nhet Galactic Empire");
                    vec.emplace_back("Ik was ook senator");
                    });

            //Originele muziek
            ix = 0;
            add_music('e', "Geef artist + titel", "orig-de_kreuners-nu_of_nooit.ogg");
            add_music('e', "Geef artist + titel", "orig-dolly_parton-i_will_alway_love_you.ogg");
            add_music('e', "Geef artist + titel", "orig-eagles-hotel_california.ogg");
            add_music('e', "Geef artist + titel", "orig-jan_de_wilde-eerste_sneeuw.ogg", 23.5);
            add_music('e', "Geef artist + titel", "orig-scorpions-wind_of_change.ogg", 19.65);
            add_music('e', "Geef artist + titel", "orig-stomae-alors_on_dance.ogg");
            add_music('e', "Geef artist + titel", "orig-volbeat-still_counting.ogg");
            add_music('e', "Geef artist + titel", "orig-urbanus-madammen_met_een_bontjas.ogg");
            add_music('e', "Geef artist + titel", "orig-channel_zero-black_fuel.ogg");
            add_music('e', "Geef artist + titel", "orig-rik_verheye_jonas-trollenknots.ogg");

            add_music('e', "Geef artist + titel", "orig-the_steve_miller_band-the_joker.ogg");

            //FaceSwap
            ix = 0;
            add_image('f', "Geef de naam van de film", "poes-ET.png");
            add_image('f', "Geef de naam van de film", "poes-lady_vagebond.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-hermelien_ron.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-regi_linda.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-nicole_hugo.png");
            add_image('f', "Geef de naam van de film", "poes-the_life_of_pi.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-smos_ingrid.png");
            add_image('f', "Geef de naam van de film", "poes-alien.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-koen_tom.png");
            add_image('f', "Geef de naam van de film", "poes-american_beauty.png");

            add_image('f', "Face-swap: wie zijn dit?", "faceswap-filip_mathilde.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-michele_barack.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-05.png");
            add_image('f', "Face-swap: wie zijn dit?", "faceswap-08.png");
            add_image('f', "Geef de naam van de film", "poes-05.png");
            add_image('f', "Geef de naam van de film", "poes-psycho.png");

            //Kids world
            ix = 0;
            add_music('g', "Geef artist + titel", "kidsworld-billie_eilish-bad_guy.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-banaan.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-jebroer-kind_van_de_duivel.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-lmfao-sexy_and_i_know_it.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-kraantje_pappie-pompen.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-lil_kleine-drang_drugs.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-dimitri_vegas-the_hum.ogg", 2*60+7);
            add_music('g', "Geef artist + titel", "kidsworld-joost-summer_vibes.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-bbnos-lalala.ogg");
            add_music('g', "Geef artist + titel", "kidsworld-dorus_cor_steyn-2_motten.ogg");

            //Droedels
            ix = 0;
            add_image('h', "Droedel", "droedel-qupido.png");
            add_image('h', "Droedel", "droedel-hersenpan.png");
            add_image('h', "Droedel", "droedel-donderwolk.png");
            add_image('h', "Droedel", "droedel-tweeling.jpg");
            add_image('h', "Droedel", "droedel-onderwijzer.png");
            add_image('h', "Droedel", "droedel-benzine.jpg");
            add_image('h', "Droedel", "droedel-omleiding.jpg");
            add_image('h', "Droedel", "droedel-inktvis.jpg");
            add_image('h', "Droedel", "droedel-klopjacht.png");
            add_image('h', "Droedel", "droedel-pindakaas.jpg");

            add_image('h', "Droedel", "droedel-opeenhoping.jpg");
            add_image('h', "Droedel", "droedel-opstootje.png");
            add_image('h', "Droedel", "droedel-watervoorziening.png");
            add_image('h', "Droedel", "droedel-cirkelzaag.png");
            add_image('h', "Droedel", "droedel-kleineren.png");
            add_image('h', "Droedel", "droedel-ding.png");

            MSS_END();
        }

        State state_ = State::Idle;
        bool change_state_(State new_state, std::string &error)
        {
            MSS_BEGIN(bool);

            if (new_state == state_)
                return true;

            MSS(!!events_, error = "Model.events_ not set");

            //Exit operations
            switch (state_)
            {
                case State::Idle:
                    break;
                case State::NewQuestion:
                    break;
                case State::Thinking:
                    break;
                case State::Answer:
                    answer_team_.clear();
                    break;
            }

            state_ = new_state;

            //Enter operations
            switch (state_)
            {
                case State::Idle:
                    events_->me_show_question(nullptr);
                    answer_team_.clear();
                    answer_order_.clear();
                    break;
                case State::NewQuestion:
                    {
                        question_ix_ = number_.value_or(question_ix_+1);
                        number_.reset();
                        answer_team_.clear();
                        answer_order_.clear();

                        auto question = get_question_(question_ix_);
                        MSS(!!question, error = std::string("there is no question ")+std::to_string(question_ix_));
                        current_question_ = *question;
                        std::cout << std::endl << "Showing question " << question_ix_ << std::endl;
                        std::cout << "  " << current_question_->image_fn << std::endl;
                        std::cout << "  " << current_question_->music_fn << std::endl;
                        events_->me_show_question(&current_question_.value());

                        return change_state_(State::Thinking, error);
                    }
                    break;
                case State::Thinking:
                    events_->me_play_music(true, false);
                    break;
                case State::Answer:
                    events_->me_play_music(false, false);
                    break;
                case State::Error:
                    break;
            }

            MSS_END();
        }

        std::string ctor_error_;
        std::string sm_error_;
        std::vector<Question> questions_;
        std::optional<Question> current_question_;

        int question_ix_ = -1;

        std::optional<unsigned int> number_;
        std::string answer_team_;
        std::string answer_order_;
        Score score_;

        Events *events_ = nullptr;
    };

} 

#endif
