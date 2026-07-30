!(function (e, t, n, s) {
    "use strict";
    function ue(e, t, n) {
        return setTimeout(ee(e, n), t);
    }
    function E(e, t, n) {
        return !!Array.isArray(e) && (f(e, n[t], n), !0);
    }
    function f(e, t, n) {
        var o;
        if (e)
            if (e.forEach) e.forEach(t, n);
            else if (e.length !== s) for (o = 0; o < e.length; ) t.call(n, e[o], o, e), o++;
            else for (o in e) e.hasOwnProperty(o) && t.call(n, e[o], o, e);
    }
    function je(t, n, s) {
        var o =
            "DEPRECATED METHOD: " +
            n +
            `
` +
            s +
            ` AT 
`;
        return function () {
            var n = new Error("get-stack-trace"),
                i =
                    n && n.stack
                        ? n.stack
                              .replace(/^[^(]+?[\n$]/gm, "")
                              .replace(/^\s+at\s+/gm, "")
                              .replace(/^Object.<anonymous>\s*\(/gm, "{anonymous}()@")
                        : "Unknown Stack Trace",
                s = e.console && (e.console.warn || e.console.log);
            return s && s.call(e.console, o, i), t.apply(this, arguments);
        };
    }
    function r(e, t, n) {
        var s,
            o = t.prototype;
        (s = e.prototype = Object.create(o)), (s.constructor = e), (s._super = o), n && v(s, n);
    }
    function ee(e, t) {
        return function () {
            return e.apply(t, arguments);
        };
    }
    function ae(e, t) {
        return typeof e == at ? e.apply(t ? t[0] || s : s, t) : e;
    }
    function _e(e, t) {
        return e === s ? t : e;
    }
    function X(e, t, n) {
        f(H(t), function (t) {
            e.addEventListener(t, n, !1);
        });
    }
    function G(e, t, n) {
        f(H(t), function (t) {
            e.removeEventListener(t, n, !1);
        });
    }
    function Oe(e, t) {
        for (; e; ) {
            if (e == t) return !0;
            e = e.parentNode;
        }
        return !1;
    }
    function w(e, t) {
        return e.indexOf(t) > -1;
    }
    function H(e) {
        return e.trim().split(/\s+/g);
    }
    function C(e, t, n) {
        if (e.indexOf && !n) return e.indexOf(t);
        for (var s = 0; s < e.length; ) {
            if ((n && e[s][n] == t) || (!n && e[s] === t)) return s;
            s++;
        }
        return -1;
    }
    function P(e) {
        return Array.prototype.slice.call(e, 0);
    }
    function xe(e, t, n) {
        for (var i, o = [], a = [], s = 0; s < e.length; )
            (i = t ? e[s][t] : e[s]), C(a, i) < 0 && o.push(e[s]), (a[s] = i), s++;
        return (
            n &&
                (o = t
                    ? o.sort(function (e, n) {
                          return e[t] > n[t];
                      })
                    : o.sort()),
            o
        );
    }
    function W(e, t) {
        for (var n, o, a = t[0].toUpperCase() + t.slice(1), i = 0; i < ve.length; ) {
            if (((n = ve[i]), (o = n ? n + a : t), o in e)) return o;
            i++;
        }
        return s;
    }
    function gt() {
        return yt++;
    }
    function Ce(t) {
        var n = t.ownerDocument || t;
        return n.defaultView || n.parentWindow || e;
    }
    function c(e, t) {
        var n = this;
        (this.manager = e),
            (this.callback = t),
            (this.element = e.element),
            (this.target = e.options.inputTarget),
            (this.domHandler = function (t) {
                ae(e.options.enable, [e]) && n.handler(t);
            }),
            this.init();
    }
    function ft(e) {
        var n,
            t = e.options.inputClass;
        return new (n = t ? t : bt ? me : vt ? B : we ? te : Y)(e, mt);
    }
    function mt(e, t, n) {
        var s = n.pointers.length,
            r = n.changedPointers.length,
            c = t & i && s - r === 0,
            l = t & (o | a) && s - r === 0;
        (n.isFirst = !!c),
            (n.isFinal = !!l),
            c && (e.session = {}),
            (n.eventType = t),
            ht(e, n),
            e.emit("hammer.input", n),
            e.recognize(n),
            (e.session.prevInput = n);
    }
    function ht(e, t) {
        var s,
            a,
            n = e.session,
            i = t.pointers,
            r = i.length;
        n.firstInput || (n.firstInput = Ee(t)),
            r > 1 && !n.firstMultiple ? (n.firstMultiple = Ee(t)) : 1 === r && (n.firstMultiple = !1);
        var c = n.firstInput,
            o = n.firstMultiple,
            l = o ? o.center : c.center,
            d = (t.center = Ae(i));
        (t.timeStamp = he()),
            (t.deltaTime = t.timeStamp - c.timeStamp),
            (t.angle = de(l, d)),
            (t.distance = V(l, d)),
            ut(n, t),
            (t.offsetDirection = $e(t.deltaX, t.deltaY)),
            (s = De(t.deltaTime, t.deltaX, t.deltaY)),
            (t.overallVelocityX = s.x),
            (t.overallVelocityY = s.y),
            (t.overallVelocity = _(s.x) > _(s.y) ? s.x : s.y),
            (t.scale = o ? Xe(o.pointers, i) : 1),
            (t.rotation = o ? Ge(o.pointers, i) : 0),
            (t.maxPointers = n.prevInput
                ? t.pointers.length > n.prevInput.maxPointers
                    ? t.pointers.length
                    : n.prevInput.maxPointers
                : t.pointers.length),
            dt(n, t),
            (a = e.element),
            Oe(t.srcEvent.target, a) && (a = t.srcEvent.target),
            (t.target = a);
    }
    function ut(e, t) {
        var n = t.center,
            s = e.offsetDelta || {},
            a = e.prevDelta || {},
            r = e.prevInput || {};
        (t.eventType !== i && r.eventType !== o) ||
            ((a = e.prevDelta = { x: r.deltaX || 0, y: r.deltaY || 0 }), (s = e.offsetDelta = { x: n.x, y: n.y })),
            (t.deltaX = a.x + (n.x - s.x)),
            (t.deltaY = a.y + (n.y - s.y));
    }
    function dt(e, t) {
        var i,
            r,
            c,
            l,
            n = e.lastInterval || t,
            d = t.timeStamp - n.timeStamp;
        if (t.eventType != a && (d > rt || n.velocity === s)) {
            var u = t.deltaX - n.deltaX,
                h = t.deltaY - n.deltaY,
                o = De(d, u, h);
            (r = o.x), (c = o.y), (i = _(o.x) > _(o.y) ? o.x : o.y), (l = $e(u, h)), (e.lastInterval = t);
        } else (i = n.velocity), (r = n.velocityX), (c = n.velocityY), (l = n.direction);
        (t.velocity = i), (t.velocityX = r), (t.velocityY = c), (t.direction = l);
    }
    function Ee(e) {
        for (var n = [], t = 0; t < e.pointers.length; )
            (n[t] = { clientX: k(e.pointers[t].clientX), clientY: k(e.pointers[t].clientY) }), t++;
        return { timeStamp: he(), pointers: n, center: Ae(n), deltaX: e.deltaX, deltaY: e.deltaY };
    }
    function Ae(e) {
        if (((t = e.length), 1 === t)) return { x: k(e[0].clientX), y: k(e[0].clientY) };
        for (var t, s = 0, o = 0, n = 0; t > n; ) (s += e[n].clientX), (o += e[n].clientY), n++;
        return { x: k(s / t), y: k(o / t) };
    }
    function De(e, t, n) {
        return { x: t / e || 0, y: n / e || 0 };
    }
    function $e(e, t) {
        return e === t ? K : _(e) >= _(t) ? (0 > e ? F : T) : 0 > t ? z : D;
    }
    function V(e, t, n) {
        n || (n = fe);
        var s = t[n[0]] - e[n[0]],
            o = t[n[1]] - e[n[1]];
        return Math.sqrt(s * s + o * o);
    }
    function de(e, t, n) {
        n || (n = fe);
        var s = t[n[0]] - e[n[0]],
            o = t[n[1]] - e[n[1]];
        return (180 * Math.atan2(o, s)) / Math.PI;
    }
    function Ge(e, t) {
        return de(t[1], t[0], L) + de(e[1], e[0], L);
    }
    function Xe(e, t) {
        return V(t[0], t[1], L) / V(e[0], e[1], L);
    }
    function Y() {
        (this.evEl = Ue), (this.evWin = Ke), (this.pressed = !1), c.apply(this, arguments);
    }
    function me() {
        (this.evEl = Ve),
            (this.evWin = Ie),
            c.apply(this, arguments),
            (this.store = this.manager.session.pointerEvents = []);
    }
    function He() {
        (this.evTarget = Ze), (this.evWin = Je), (this.started = !1), c.apply(this, arguments);
    }
    function et(e, t) {
        var n = P(e.touches),
            s = P(e.changedTouches);
        return t & (o | a) && (n = xe(n.concat(s), "identifier", !0)), [n, s];
    }
    function B() {
        (this.evTarget = Re), (this.targetIds = {}), c.apply(this, arguments);
    }
    function tt(e, t) {
        var s = P(e.touches),
            r = this.targetIds;
        if (t & (i | j) && 1 === s.length) return (r[s[0].identifier] = !0), [s, s];
        var n,
            c,
            l = P(e.changedTouches),
            d = [],
            u = this.target;
        if (
            ((c = s.filter(function (e) {
                return Oe(e.target, u);
            })),
            t === i)
        )
            for (n = 0; n < c.length; ) (r[c[n].identifier] = !0), n++;
        for (n = 0; n < l.length; ) r[l[n].identifier] && d.push(l[n]), t & (o | a) && delete r[l[n].identifier], n++;
        return d.length ? [xe(c.concat(d), "identifier", !0), d] : void 0;
    }
    function te() {
        c.apply(this, arguments);
        var e = ee(this.handler, this);
        (this.touch = new B(this.manager, e)),
            (this.mouse = new Y(this.manager, e)),
            (this.primaryTouch = null),
            (this.lastTouches = []);
    }
    function nt(e, t) {
        e & i
            ? ((this.primaryTouch = t.changedPointers[0].identifier), Ne.call(this, t))
            : e & (o | a) && Ne.call(this, t);
    }
    function Ne(e) {
        if (((t = e.changedPointers[0]), t.identifier === this.primaryTouch)) {
            (n = { x: t.clientX, y: t.clientY }), this.lastTouches.push(n);
            var t,
                n,
                s = this.lastTouches,
                o = function () {
                    var e = s.indexOf(n);
                    e > -1 && s.splice(e, 1);
                };
            setTimeout(o, Le);
        }
    }
    function st(e) {
        for (var s = e.srcEvent.clientX, o = e.srcEvent.clientY, t = 0; t < this.lastTouches.length; t++) {
            var n = this.lastTouches[t],
                i = Math.abs(s - n.x),
                a = Math.abs(o - n.y);
            if (Z >= i && Z >= a) return !0;
        }
        return !1;
    }
    function ie(e, t) {
        (this.manager = e), this.set(t);
    }
    function ot(e) {
        if (w(e, O)) return O;
        var t = w(e, A),
            n = w(e, S);
        return t && n ? O : t || n ? (t ? A : S) : w(e, J) ? J : Se;
    }
    function it() {
        if (!Te) return !1;
        var t = {},
            n = e.CSS && e.CSS.supports;
        return (
            ["auto", "manipulation", "pan-y", "pan-x", "pan-x pan-y", "none"].forEach(function (s) {
                t[s] = !n || e.CSS.supports("touch-action", s);
            }),
            t
        );
    }
    function m(e) {
        (this.options = v({}, this.defaults, e || {})),
            (this.id = gt()),
            (this.manager = null),
            (this.options.enable = _e(this.options.enable, !0)),
            (this.state = I),
            (this.simultaneous = {}),
            (this.requireFail = []);
    }
    function Fe(e) {
        return e & M ? "cancel" : e & b ? "end" : e & x ? "move" : e & l ? "start" : "";
    }
    function Me(e) {
        return e == D ? "down" : e == z ? "up" : e == F ? "left" : e == T ? "right" : "";
    }
    function q(e, t) {
        var n = t.manager;
        return n ? n.get(e) : e;
    }
    function d() {
        m.apply(this, arguments);
    }
    function U() {
        d.apply(this, arguments), (this.pX = null), (this.pY = null);
    }
    function ne() {
        d.apply(this, arguments);
    }
    function se() {
        m.apply(this, arguments), (this._timer = null), (this._input = null);
    }
    function re() {
        d.apply(this, arguments);
    }
    function Q() {
        d.apply(this, arguments);
    }
    function $() {
        m.apply(this, arguments),
            (this.pTime = !1),
            (this.pCenter = !1),
            (this._timer = null),
            (this._input = null),
            (this.count = 0);
    }
    function g(e, t) {
        return (t = t || {}), (t.recognizers = _e(t.recognizers, g.defaults.preset)), new ce(e, t);
    }
    function ce(e, t) {
        (this.options = v({}, g.defaults, t || {})),
            (this.options.inputTarget = this.options.inputTarget || e),
            (this.handlers = {}),
            (this.session = {}),
            (this.recognizers = []),
            (this.oldCssProps = {}),
            (this.element = e),
            (this.input = ft(this)),
            (this.touchAction = new ie(this, this.options.touchAction)),
            ke(this, !0),
            f(
                this.options.recognizers,
                function (e) {
                    var t = this.add(new e[0](e[1]));
                    e[2] && t.recognizeWith(e[2]), e[3] && t.requireFailure(e[3]);
                },
                this
            );
    }
    function ke(e, t) {
        var n,
            s = e.element;
        s.style &&
            (f(e.options.cssProps, function (o, i) {
                (n = W(s.style, i)),
                    t ? ((e.oldCssProps[n] = s.style[n]), (s.style[n] = o)) : (s.style[n] = e.oldCssProps[n] || "");
            }),
            t || (e.oldCssProps = {}));
    }
    function pt(e, n) {
        var s = t.createEvent("Event");
        s.initEvent(e, !0, !0), (s.gesture = n), n.target.dispatchEvent(s);
    }
    var Z,
        ve = ["", "webkit", "Moz", "MS", "ms", "o"],
        wt = t.createElement("div"),
        at = "function",
        k = Math.round,
        _ = Math.abs,
        he = Date.now,
        Pe,
        Re,
        Le,
        ye,
        oe,
        ge,
        v =
            "function" != typeof Object.assign
                ? function (e) {
                      if (e === s || null === e) throw new TypeError("Cannot convert undefined or null to object");
                      for (var t, o, i = Object(e), n = 1; n < arguments.length; n++)
                          if (((t = arguments[n]), t !== s && null !== t))
                              for (o in t) t.hasOwnProperty(o) && (i[o] = t[o]);
                      return i;
                  }
                : Object.assign,
        be = je(
            function (e, t, n) {
                for (var i = Object.keys(t), o = 0; o < i.length; )
                    (!n || (n && e[i[o]] === s)) && (e[i[o]] = t[i[o]]), o++;
                return e;
            },
            "extend",
            "Use `assign`."
        ),
        _t = je(
            function (e, t) {
                return be(e, t, !0);
            },
            "merge",
            "Use `assign`."
        ),
        yt = 1,
        jt = /mobile|tablet|ip(ad|hone|od)|android/i,
        we = "ontouchstart" in e,
        bt = W(e, "PointerEvent") !== s,
        vt = we && jt.test(navigator.userAgent),
        N = "touch",
        lt = "pen",
        le = "mouse",
        ct = "kinect",
        rt = 25,
        i = 1,
        j = 2,
        o = 4,
        a = 8,
        K = 1,
        F = 2,
        T = 4,
        z = 8,
        D = 16,
        u = F | T,
        y = z | D,
        Be = u | y,
        fe = ["x", "y"],
        L = ["clientX", "clientY"];
    c.prototype = {
        handler: function () {},
        init: function () {
            this.evEl && X(this.element, this.evEl, this.domHandler),
                this.evTarget && X(this.target, this.evTarget, this.domHandler),
                this.evWin && X(Ce(this.element), this.evWin, this.domHandler);
        },
        destroy: function () {
            this.evEl && G(this.element, this.evEl, this.domHandler),
                this.evTarget && G(this.target, this.evTarget, this.domHandler),
                this.evWin && G(Ce(this.element), this.evWin, this.domHandler);
        },
    };
    var We = { mousedown: i, mousemove: j, mouseup: o },
        Ue = "mousedown",
        Ke = "mousemove mouseup";
    r(Y, c, {
        handler: function (e) {
            var t = We[e.type];
            t & i && 0 === e.button && (this.pressed = !0),
                t & j && 1 !== e.which && (t = o),
                this.pressed &&
                    (t & o && (this.pressed = !1),
                    this.callback(this.manager, t, {
                        pointers: [e],
                        changedPointers: [e],
                        pointerType: le,
                        srcEvent: e,
                    }));
        },
    });
    var qe = { pointerdown: i, pointermove: j, pointerup: o, pointercancel: a, pointerout: a },
        Ye = { 2: N, 3: lt, 4: le, 5: ct },
        Ve = "pointerdown",
        Ie = "pointermove pointerup pointercancel";
    e.MSPointerEvent && !e.PointerEvent && ((Ve = "MSPointerDown"), (Ie = "MSPointerMove MSPointerUp MSPointerCancel")),
        r(me, c, {
            handler: function (e) {
                var t = this.store,
                    r = !1,
                    l = e.type.toLowerCase().replace("ms", ""),
                    s = qe[l],
                    c = Ye[e.pointerType] || e.pointerType,
                    d = c == N,
                    n = C(t, e.pointerId, "pointerId");
                s & i && (0 === e.button || d) ? 0 > n && (t.push(e), (n = t.length - 1)) : s & (o | a) && (r = !0),
                    0 > n ||
                        ((t[n] = e),
                        this.callback(this.manager, s, {
                            pointers: t,
                            changedPointers: [e],
                            pointerType: c,
                            srcEvent: e,
                        }),
                        r && t.splice(n, 1));
            },
        });
    var Qe = { touchstart: i, touchmove: j, touchend: o, touchcancel: a },
        Ze = "touchstart",
        Je = "touchstart touchmove touchend touchcancel";
    r(He, c, {
        handler: function (e) {
            var t,
                n = Qe[e.type];
            n === i && (this.started = !0),
                this.started &&
                    ((t = et.call(this, e, n)),
                    n & (o | a) && t[0].length - t[1].length === 0 && (this.started = !1),
                    this.callback(this.manager, n, {
                        pointers: t[0],
                        changedPointers: t[1],
                        pointerType: N,
                        srcEvent: e,
                    }));
        },
    }),
        (Pe = { touchstart: i, touchmove: j, touchend: o, touchcancel: a }),
        (Re = "touchstart touchmove touchend touchcancel"),
        r(B, c, {
            handler: function (e) {
                var n = Pe[e.type],
                    t = tt.call(this, e, n);
                t &&
                    this.callback(this.manager, n, {
                        pointers: t[0],
                        changedPointers: t[1],
                        pointerType: N,
                        srcEvent: e,
                    });
            },
        }),
        (Le = 2500),
        (Z = 25),
        r(te, c, {
            handler: function (e, t, n) {
                var o = n.pointerType == N,
                    s = n.pointerType == le;
                if (!(s && n.sourceCapabilities && n.sourceCapabilities.firesTouchEvents)) {
                    if (o) nt.call(this, t, n);
                    else if (s && st.call(this, n)) return;
                    this.callback(e, t, n);
                }
            },
            destroy: function () {
                this.touch.destroy(), this.mouse.destroy();
            },
        });
    var ze = W(wt.style, "touchAction"),
        Te = ze !== s,
        pe = "compute",
        Se = "auto",
        J = "manipulation",
        O = "none",
        A = "pan-x",
        S = "pan-y",
        R = it();
    ie.prototype = {
        set: function (e) {
            e == pe && (e = this.compute()),
                Te && this.manager.element.style && R[e] && (this.manager.element.style[ze] = e),
                (this.actions = e.toLowerCase().trim());
        },
        update: function () {
            this.set(this.manager.options.touchAction);
        },
        compute: function () {
            var e = [];
            return (
                f(this.manager.recognizers, function (t) {
                    ae(t.options.enable, [t]) && (e = e.concat(t.getTouchAction()));
                }),
                ot(e.join(" "))
            );
        },
        preventDefaults: function (e) {
            var n = e.srcEvent,
                s = e.offsetDirection;
            if (this.manager.session.prevented) return void n.preventDefault();
            var t = this.actions,
                o = w(t, O) && !R[O],
                i = w(t, S) && !R[S],
                a = w(t, A) && !R[A];
            if (o) {
                var r = 1 === e.pointers.length,
                    c = e.distance < 2,
                    l = e.deltaTime < 250;
                if (r && c && l) return;
            }
            return a && i ? void 0 : o || (i && s & u) || (a && s & y) ? this.preventSrc(n) : void 0;
        },
        preventSrc: function (e) {
            (this.manager.session.prevented = !0), e.preventDefault();
        },
    };
    var I = 1,
        l = 2,
        x = 4,
        b = 8,
        p = b,
        M = 16,
        h = 32;
    (m.prototype = {
        defaults: {},
        set: function (e) {
            return v(this.options, e), this.manager && this.manager.touchAction.update(), this;
        },
        recognizeWith: function (e) {
            if (E(e, "recognizeWith", this)) return this;
            var t = this.simultaneous;
            return (e = q(e, this)), t[e.id] || ((t[e.id] = e), e.recognizeWith(this)), this;
        },
        dropRecognizeWith: function (e) {
            return E(e, "dropRecognizeWith", this) ? this : ((e = q(e, this)), delete this.simultaneous[e.id], this);
        },
        requireFailure: function (e) {
            if (E(e, "requireFailure", this)) return this;
            var t = this.requireFail;
            return (e = q(e, this)), -1 === C(t, e) && (t.push(e), e.requireFailure(this)), this;
        },
        dropRequireFailure: function (e) {
            if (E(e, "dropRequireFailure", this)) return this;
            e = q(e, this);
            var t = C(this.requireFail, e);
            return t > -1 && this.requireFail.splice(t, 1), this;
        },
        hasRequireFailures: function () {
            return this.requireFail.length > 0;
        },
        canRecognizeWith: function (e) {
            return !!this.simultaneous[e.id];
        },
        emit: function (e) {
            function t(t) {
                n.manager.emit(t, e);
            }
            var n = this,
                s = this.state;
            b > s && t(n.options.event + Fe(s)),
                t(n.options.event),
                e.additionalEvent && t(e.additionalEvent),
                s >= b && t(n.options.event + Fe(s));
        },
        tryEmit: function (e) {
            return this.canEmit() ? this.emit(e) : void (this.state = h);
        },
        canEmit: function () {
            for (var e = 0; e < this.requireFail.length; ) {
                if (!(this.requireFail[e].state & (h | I))) return !1;
                e++;
            }
            return !0;
        },
        recognize: function (e) {
            var t = v({}, e);
            return ae(this.options.enable, [this, t])
                ? (this.state & (p | M | h) && (this.state = I),
                  (this.state = this.process(t)),
                  void (this.state & (l | x | b | M) && this.tryEmit(t)))
                : (this.reset(), void (this.state = h));
        },
        process: function () {},
        getTouchAction: function () {},
        reset: function () {},
    }),
        r(d, m, {
            defaults: { pointers: 1 },
            attrTest: function (e) {
                var t = this.options.pointers;
                return 0 === t || e.pointers.length === t;
            },
            process: function (e) {
                var t = this.state,
                    n = e.eventType,
                    s = t & (l | x),
                    i = this.attrTest(e);
                return s && (n & a || !i) ? t | M : s || i ? (n & o ? t | b : t & l ? t | x : l) : h;
            },
        }),
        r(U, d, {
            defaults: { event: "pan", threshold: 10, pointers: 1, direction: Be },
            getTouchAction: function () {
                var t = this.options.direction,
                    e = [];
                return t & u && e.push(S), t & y && e.push(A), e;
            },
            directionTest: function (e) {
                var n = this.options,
                    s = !0,
                    o = e.distance,
                    t = e.direction,
                    i = e.deltaX,
                    a = e.deltaY;
                return (
                    t & n.direction ||
                        (n.direction & u
                            ? ((t = 0 === i ? K : 0 > i ? F : T), (s = i != this.pX), (o = Math.abs(e.deltaX)))
                            : ((t = 0 === a ? K : 0 > a ? z : D), (s = a != this.pY), (o = Math.abs(e.deltaY)))),
                    (e.direction = t),
                    s && o > n.threshold && t & n.direction
                );
            },
            attrTest: function (e) {
                return (
                    d.prototype.attrTest.call(this, e) &&
                    (this.state & l || (!(this.state & l) && this.directionTest(e)))
                );
            },
            emit: function (e) {
                (this.pX = e.deltaX), (this.pY = e.deltaY);
                var t = Me(e.direction);
                t && (e.additionalEvent = this.options.event + t), this._super.emit.call(this, e);
            },
        }),
        r(ne, d, {
            defaults: { event: "pinch", threshold: 0, pointers: 2 },
            getTouchAction: function () {
                return [O];
            },
            attrTest: function (e) {
                return (
                    this._super.attrTest.call(this, e) &&
                    (Math.abs(e.scale - 1) > this.options.threshold || this.state & l)
                );
            },
            emit: function (e) {
                if (1 !== e.scale) {
                    var t = e.scale < 1 ? "in" : "out";
                    e.additionalEvent = this.options.event + t;
                }
                this._super.emit.call(this, e);
            },
        }),
        r(se, m, {
            defaults: { event: "press", pointers: 1, time: 251, threshold: 9 },
            getTouchAction: function () {
                return [Se];
            },
            process: function (e) {
                var t = this.options,
                    n = e.pointers.length === t.pointers,
                    s = e.distance < t.threshold,
                    r = e.deltaTime > t.time;
                if (((this._input = e), !s || !n || (e.eventType & (o | a) && !r))) this.reset();
                else if (e.eventType & i)
                    this.reset(),
                        (this._timer = ue(
                            function () {
                                (this.state = p), this.tryEmit();
                            },
                            t.time,
                            this
                        ));
                else if (e.eventType & o) return p;
                return h;
            },
            reset: function () {
                clearTimeout(this._timer);
            },
            emit: function (e) {
                this.state === p &&
                    (e && e.eventType & o
                        ? this.manager.emit(this.options.event + "up", e)
                        : ((this._input.timeStamp = he()), this.manager.emit(this.options.event, this._input)));
            },
        }),
        r(re, d, {
            defaults: { event: "rotate", threshold: 0, pointers: 2 },
            getTouchAction: function () {
                return [O];
            },
            attrTest: function (e) {
                return (
                    this._super.attrTest.call(this, e) &&
                    (Math.abs(e.rotation) > this.options.threshold || this.state & l)
                );
            },
        }),
        r(Q, d, {
            defaults: { event: "swipe", threshold: 10, velocity: 0.3, direction: u | y, pointers: 1 },
            getTouchAction: function () {
                return U.prototype.getTouchAction.call(this);
            },
            attrTest: function (e) {
                var t,
                    n = this.options.direction;
                return (
                    n & (u | y)
                        ? (t = e.overallVelocity)
                        : n & u
                          ? (t = e.overallVelocityX)
                          : n & y && (t = e.overallVelocityY),
                    this._super.attrTest.call(this, e) &&
                        n & e.offsetDirection &&
                        e.distance > this.options.threshold &&
                        e.maxPointers == this.options.pointers &&
                        _(t) > this.options.velocity &&
                        e.eventType & o
                );
            },
            emit: function (e) {
                var t = Me(e.offsetDirection);
                t && this.manager.emit(this.options.event + t, e), this.manager.emit(this.options.event, e);
            },
        }),
        r($, m, {
            defaults: { event: "tap", pointers: 1, taps: 1, interval: 300, time: 250, threshold: 9, posThreshold: 10 },
            getTouchAction: function () {
                return [J];
            },
            process: function (e) {
                var n,
                    s,
                    a,
                    t = this.options,
                    r = e.pointers.length === t.pointers,
                    c = e.distance < t.threshold,
                    d = e.deltaTime < t.time;
                if ((this.reset(), e.eventType & i && 0 === this.count)) return this.failTimeout();
                if (c && d && r) {
                    if (e.eventType != o) return this.failTimeout();
                    if (
                        ((n = !this.pTime || e.timeStamp - this.pTime < t.interval),
                        (s = !this.pCenter || V(this.pCenter, e.center) < t.posThreshold),
                        (this.pTime = e.timeStamp),
                        (this.pCenter = e.center),
                        s && n ? (this.count += 1) : (this.count = 1),
                        (this._input = e),
                        (a = this.count % t.taps),
                        0 === a)
                    )
                        return this.hasRequireFailures()
                            ? ((this._timer = ue(
                                  function () {
                                      (this.state = p), this.tryEmit();
                                  },
                                  t.interval,
                                  this
                              )),
                              l)
                            : p;
                }
                return h;
            },
            failTimeout: function () {
                return (
                    (this._timer = ue(
                        function () {
                            this.state = h;
                        },
                        this.options.interval,
                        this
                    )),
                    h
                );
            },
            reset: function () {
                clearTimeout(this._timer);
            },
            emit: function () {
                this.state == p &&
                    ((this._input.tapCount = this.count), this.manager.emit(this.options.event, this._input));
            },
        }),
        (g.VERSION = "2.0.7"),
        (g.defaults = {
            domEvents: !1,
            touchAction: pe,
            enable: !0,
            inputTarget: null,
            inputClass: null,
            preset: [
                [re, { enable: !1 }],
                [ne, { enable: !1 }, ["rotate"]],
                [Q, { direction: u }],
                [U, { direction: u }, ["swipe"]],
                [$],
                [$, { event: "doubletap", taps: 2 }, ["tap"]],
                [se],
            ],
            cssProps: {
                userSelect: "none",
                touchSelect: "none",
                touchCallout: "none",
                contentZooming: "none",
                userDrag: "none",
                tapHighlightColor: "rgba(0,0,0,0)",
            },
        }),
        (ye = 1),
        (oe = 2),
        (ce.prototype = {
            set: function (e) {
                return (
                    v(this.options, e),
                    e.touchAction && this.touchAction.update(),
                    e.inputTarget && (this.input.destroy(), (this.input.target = e.inputTarget), this.input.init()),
                    this
                );
            },
            stop: function (e) {
                this.session.stopped = e ? oe : ye;
            },
            recognize: function (e) {
                if (((s = this.session), !s.stopped)) {
                    this.touchAction.preventDefaults(e);
                    var n,
                        s,
                        o,
                        i = this.recognizers,
                        t = s.curRecognizer;
                    (!t || (t && t.state & p)) && (t = s.curRecognizer = null);
                    for (o = 0; o < i.length; )
                        (n = i[o]),
                            s.stopped === oe || (t && n != t && !n.canRecognizeWith(t)) ? n.reset() : n.recognize(e),
                            !t && n.state & (l | x | b) && (t = s.curRecognizer = n),
                            o++;
                }
            },
            get: function (e) {
                if (e instanceof m) return e;
                for (var n = this.recognizers, t = 0; t < n.length; t++) if (n[t].options.event == e) return n[t];
                return null;
            },
            add: function (e) {
                if (E(e, "add", this)) return this;
                var t = this.get(e.options.event);
                return t && this.remove(t), this.recognizers.push(e), (e.manager = this), this.touchAction.update(), e;
            },
            remove: function (e) {
                if (E(e, "remove", this)) return this;
                if ((e = this.get(e))) {
                    var t = this.recognizers,
                        n = C(t, e);
                    -1 !== n && (t.splice(n, 1), this.touchAction.update());
                }
                return this;
            },
            on: function (e, t) {
                if (e !== s && t !== s) {
                    var n = this.handlers;
                    return (
                        f(H(e), function (e) {
                            (n[e] = n[e] || []), n[e].push(t);
                        }),
                        this
                    );
                }
            },
            off: function (e, t) {
                if (e !== s) {
                    var n = this.handlers;
                    return (
                        f(H(e), function (e) {
                            t ? n[e] && n[e].splice(C(n[e], t), 1) : delete n[e];
                        }),
                        this
                    );
                }
            },
            emit: function (e, t) {
                this.options.domEvents && pt(e, t);
                var s,
                    n = this.handlers[e] && this.handlers[e].slice();
                if (n && n.length) {
                    (t.type = e),
                        (t.preventDefault = function () {
                            t.srcEvent.preventDefault();
                        });
                    for (s = 0; s < n.length; ) n[s](t), s++;
                }
            },
            destroy: function () {
                this.element && ke(this, !1),
                    (this.handlers = {}),
                    (this.session = {}),
                    this.input.destroy(),
                    (this.element = null);
            },
        }),
        v(g, {
            INPUT_START: i,
            INPUT_MOVE: j,
            INPUT_END: o,
            INPUT_CANCEL: a,
            STATE_POSSIBLE: I,
            STATE_BEGAN: l,
            STATE_CHANGED: x,
            STATE_ENDED: b,
            STATE_RECOGNIZED: p,
            STATE_CANCELLED: M,
            STATE_FAILED: h,
            DIRECTION_NONE: K,
            DIRECTION_LEFT: F,
            DIRECTION_RIGHT: T,
            DIRECTION_UP: z,
            DIRECTION_DOWN: D,
            DIRECTION_HORIZONTAL: u,
            DIRECTION_VERTICAL: y,
            DIRECTION_ALL: Be,
            Manager: ce,
            Input: c,
            TouchAction: ie,
            TouchInput: B,
            MouseInput: Y,
            PointerEventInput: me,
            TouchMouseInput: te,
            SingleTouchInput: He,
            Recognizer: m,
            AttrRecognizer: d,
            Tap: $,
            Pan: U,
            Swipe: Q,
            Pinch: ne,
            Rotate: re,
            Press: se,
            on: X,
            off: G,
            each: f,
            merge: _t,
            extend: be,
            assign: v,
            inherit: r,
            bindFn: ee,
            prefixed: W,
        }),
        (ge = "undefined" != typeof e ? e : "undefined" != typeof self ? self : {}),
        (ge.Hammer = g),
        "function" == typeof define && define.amd
            ? define(function () {
                  return g;
              })
            : "undefined" != typeof module && module.exports
              ? (module.exports = g)
              : (e[n] = g);
})(window, document, "Hammer"),
    (() => {
        let d,
            pe = !1,
            Se,
            Ue,
            te,
            A,
            $,
            j,
            z,
            Re,
            J = null,
            k,
            Ot,
            y,
            gt,
            O,
            Ce,
            ge,
            yt,
            nn = 0,
            h,
            ie,
            X = null,
            Pe = null,
            a,
            K,
            R = [],
            v = null,
            xe = 0,
            ce,
            Te,
            Kt = !1,
            ti,
            Nt,
            It = 0;
        const ar = [
                "commander-v2",
                "kill-v2",
                "partners-v2",
                "unkill-v2",
                "history",
                "poison",
                "tax",
                "tax-1",
                "tax-2",
                "storm",
                "xp",
                "energy",
                "rad",
                "aC",
                "bC",
                "cC",
                "W",
                "U",
                "B",
                "R",
                "G",
                "C",
                "speed",
                "dice",
                "search",
                "edit",
                "check-white",
                "planechase",
                "planechase-settings",
                "previous-card",
                "planeswalk",
                "chaos",
                "flip",
                "archenemy",
                "day",
                "night",
                "monarch",
                "initiative",
                "day-font",
                "night-font",
                "blood",
                "clue",
                "food",
                "gold",
                "map",
                "treasure",
                "color-picker",
                "plus",
                "dagger",
                "load",
                "bounty",
                "player-avatar",
                "player-avatar-white",
                "duration",
                "turns",
                "player-avatar-list",
                "calendar",
                "triangle-up",
                "triangle-down",
                "winner-star",
            ],
            $t = [
                "Are you sure that's a card name? No results found.",
                "You made this card name up, right? Right? No results found.",
                "That card name must be from an alternate universe. No results found!",
                "Did you just invent a new card? Because we can't find it!",
                "This card must be in a hidden set... No results found.",
                "Maybe double-check that spelling? No results for that card name.",
                "We searched everywhere, but it looks like that card doesn't exist!",
                "Is this a secret test card? Because we can't find it.",
                "Are you playing an Un-set? No results for that card name.",
                "Did you just pull this name out of a hat? Because it's not in the database!",
            ],
            eo = [
                "Blocked by a wall.",
                "Blocked out.",
                "Bluffed out.",
                "Boom!",
                "Brick walled.",
                "Burned out.",
                "Bye-bye.",
                "Cascade fail.",
                "Commander down.",
                "Conceded.",
                "Countered.",
                "Couldn’t keep up.",
                "Couldn’t recover.",
                "Decked out.",
                "Defeated.",
                "Delved too deep.",
                "Devoured.",
                "Drawn into oblivion.",
                "Dredged out.",
                "Emptied the library.",
                "Exiled.",
                "Failed combo.",
                "Fatal misplay.",
                "Flooded.",
                "Forgot to untap.",
                "GG, no re.",
                "Game over.",
                "Got janked.",
                "Hexproof but hopeless.",
                "I won! Not.",
                "KO'd!",
                "Lightning Bolted.",
                "Lost the coin flip.",
                "Lost the stack.",
                "Lost to RNG.",
                "Lost.",
                "Mana burned.",
                "Mana screwed.",
                "Milling to death.",
                "Misclicked!",
                "Missed opportunity.",
                "Missed the combo.",
                "Missed the land drop.",
                "Missed trigger.",
                "Mulligan fail.",
                "Next time?",
                "No answers left.",
                "No board wipes.",
                "No miracles.",
                "No more draws.",
                "No more lifelink.",
                "No more tokens.",
                "No more topdecks.",
                "Not enough creatures.",
                "Not enough lands.",
                "Not enough mana.",
                "Oops!",
                "Ouch.",
                "Out of responses.",
                "Out of spells.",
                "Outclassed.",
                "Overrun.",
                "Planeswalked away.",
                "Poof!",
                "Punted the game.",
                "R.I.P.",
                "Ran out of gas.",
                "Sacrificed.",
                "Scooped.",
                "Scry harder.",
                "Shuffled off.",
                "Stuck in the upkeep.",
                "Tapped out.",
                "Toasted.",
                "Too many counters.",
                "Too many puns.",
                "Topdecked a land.",
                "Trapped in the graveyard.",
                "Unstable.",
                "Unwound.",
                "Vanquished.",
                "Wrong line.",
                "y u no win?",
            ],
            Vs = [
                "Aetherflux success.",
                "Affinity for victory.",
                "All the right draws.",
                "Annihilator supreme.",
                "Ascended to victory.",
                "Avenger of Zendikar.",
                "Best card draw!",
                "Best of three.",
                "Board wipe pro.",
                "Cascade of wins.",
                "Chained to victory.",
                "Chalice of the winner.",
                "Champion!",
                "Combo King/Queen.",
                "Devotion to victory.",
                "Dominated the battlefield.",
                "Drew all the gas.",
                "Eldrazi annihilation.",
                "Endgame conqueror.",
                "Epic win.",
                "Exalted champion.",
                "Exiled to victory.",
                "Flash of brilliance.",
                "Heroic victory.",
                "I control your fate!",
                "Infectiously victorious.",
                "Invincible!",
                "Jace'd it!",
                "Legendary win.",
                "Library victor.",
                "MVP.",
                "Master of the Multiverse.",
                "Master of the stack.",
                "Miracle win!",
                "Nicol Bolas’d!",
                "No counter for this victory.",
                "No more phases needed.",
                "Out-scryed and out-cried.",
                "Outplayed.",
                "Overrun them!",
                "Pondered into victory.",
                "Protection from losing.",
                "Reanimated to victory.",
                "Scapeshifted to success.",
                "Scry-tastic!",
                "Storm count: Infinity.",
                "Tapped for the win.",
                "Topdecked the win.",
                "Trample to triumph.",
                "Triumphant topdeck.",
                "Tutored the win.",
                "Uncounterable triumph.",
                "Undaunted champion.",
                "Unstoppable force.",
                "Vigilantly victorious.",
                "Winner, winner, Goblin dinner!",
            ],
            lo = [
                "Death by poison.",
                "Infect-checked.",
                "Ten counters. That's game.",
                "Poison counters say GG.",
                "Infect damage: Fatal!",
            ],
            Ss = [
                "That's 21 damage.",
                "Slain by their commander.",
                "Another Voltron win.",
                "21 to the face.",
                "Voltron did its thing.",
            ],
            dn = "https://api.scryfall.com/cards/search?",
            w = { once: !0 },
            tt = "active",
            V = "inactive",
            o = "click",
            st = "touchstart",
            qe = "touchend",
            _t = "touchmove",
            zs = "touchcancel",
            pt = "clicked",
            x = "animationend",
            oe = "transitionend",
            Z = "hide",
            Et = "show",
            Ge = "none",
            E = "fade-out",
            be = "killed",
            lt = "winner",
            N = "btn",
            Y = "left",
            ee = "right",
            fe = "up",
            de = "down",
            Ee = "lock",
            p = "text",
            I = "disabled",
            Me = "next",
            Ve = "current",
            rt = "previous",
            F = "screen_view",
            M = "input",
            H = "red",
            D = "blue",
            Q = "green",
            B = "yellow",
            nt = "purple",
            re = "orange",
            q = "lightPurple",
            Ft = "grey",
            ts = "lightGrey",
            ke = "sand",
            es = "white",
            Un = "pink",
            ae = "upright",
            Fe = "upside-down",
            l = "rotate-right",
            u = "rotate-left",
            ve = window,
            m = document,
            Oe = m.documentElement,
            n = (e, t = tt) => e.classList.add(t),
            r = (e, t = tt) => e.classList.remove(t),
            ua = (e, t) => t.forEach((t) => n(e, t)),
            f = (e, t = tt) => e.classList.toggle(t),
            _ = (e, t = tt) => e.classList.contains(t),
            ut = (e, t, s) => {
                s ? n(e, t) : r(e, t);
            },
            e = (e = [], t = "div") => {
                let s = m.createElement(t);
                return e.forEach((e) => n(s, e)), s;
            },
            t = (e, t) => e.forEach((e) => t.appendChild(e)),
            Ao = (e, t) => {
                e.reverse().forEach((e) => t.insertBefore(e, t.firstChild));
            },
            bt = (e, t, s = 200) => {
                if (!e) return;
                n(e, t),
                    setTimeout(() => {
                        r(e, t);
                    }, s);
            },
            C = (e, t, n) => (e.style[t] = n),
            g = (e, t, n = Oe) => n.style.setProperty(e, t),
            We = (e) => (e && e.startsWith("#") ? e : `var(--${e})`),
            $e = (e) => {
                if (!e) return !1;
                if (e.startsWith("#")) {
                    const t = e.replace(/^#/, ""),
                        n = parseInt(t.substr(0, 2), 16),
                        s = parseInt(t.substr(2, 2), 16),
                        o = parseInt(t.substr(4, 2), 16),
                        i = (n * 299 + s * 587 + o * 114) / 1e3;
                    return i < 128;
                }
                return !1;
            },
            la = (e) => g("--vh", `${e}px`),
            ia = (e) => g("--vw", `${e}px`),
            na = (e, t) => g("--aspect-ratio", `${t / e}`),
            zn = () => {
                const e = pe ? ve.screen.height : ve.innerHeight,
                    t = pe ? ve.screen.width : ve.innerWidth;
                la(e), ia(t), na(e, t);
            },
            ot = (e) => (e.innerHTML = ""),
            s = (e, t) => (e.textContent = t),
            S = (s, o) => {
                ot(s);
                const i = o.toString(),
                    a = (t) => {
                        const s = e(["font"]),
                            o = t === "+" ? "char-plus" : t === "-" ? "char-minus" : isNaN(t) ? "" : `char-${t}`;
                        return o && n(s, o), s;
                    },
                    r = Array.from(i).map(a);
                t(r, s);
            },
            on = () => ve.location.reload(),
            i = (e, t = o, n = "Button") => {
                if (typeof gtag == "function" && !pe) {
                    const s = t === F ? { screen_name: e } : { event_category: n, event_label: e };
                    gtag("event", t, s);
                }
            },
            fn = (e, t = !0) => {
                e.forEach((e) => {
                    const n = t ? `/images/${e}.svg` : e;
                    new Image().src = n;
                });
            },
            Wo = (n, o) => {
                const r = n.clientWidth * 0.84,
                    i = e();
                C(i, "position", "absolute"), C(i, "visibility", "hidden"), C(i, "white-space", "nowrap"), t([i], d);
                let a = 100;
                for (C(i, "fontSize", `${a}px`), s(i, o); i.offsetWidth > r && a > 0; ) a--, C(i, "fontSize", `${a}px`);
                return i.remove(), a;
            },
            Jo = (e) => g("--font-size-commander", `${Wo(e.children[0], "Commander")}px`, e),
            gi = (e) => {
                g("--font-size-entry", `${Wo(e.querySelector(".option-entry.background"), "Background")}px`, e);
            },
            An = (e) => ve.addEventListener("resize", e),
            li = ["pointerdown", o, st],
            we = [
                H,
                "darkRed",
                Un,
                re,
                B,
                "brightGreen",
                "babyGreen",
                ke,
                Q,
                "seaGreen",
                D,
                "babyBlue",
                "lightBlue",
                "sandBlue",
                q,
                nt,
                Ft,
                ts,
                es,
            ],
            ln = "top",
            Je = "sides",
            mt = "bottom",
            Fn = "sides-vertical",
            W = { single: mt, multi: Fn },
            G = {
                p1: {
                    portrait: {
                        key: "portrait",
                        rotation: [ae],
                        colors: [D],
                        btns: { single: mt, multi: Je },
                        dir: [0],
                    },
                    landscape: {
                        key: "landscape",
                        rotation: [u],
                        colors: [D],
                        btns: { single: mt, multi: Je },
                        dir: [0],
                    },
                },
                p2: {
                    "landscape-landscape": {
                        key: "landscape-landscape",
                        rotation: [Fe, ae],
                        colors: [D, H],
                        btns: { single: mt, multi: Je },
                        dir: [1, 0],
                    },
                    "landscape-landscape-uni": {
                        key: "landscape-landscape-uni",
                        rotation: [ae, ae],
                        colors: [D, H],
                        btns: { single: mt, multi: Je },
                        dir: [1, 0],
                    },
                    "landscape-landscape-sides": {
                        key: "landscape-landscape-sides",
                        rotation: [l, l],
                        colors: [D, H],
                        btns: W,
                        dir: [1, 0],
                    },
                    "portrait-portrait": {
                        key: "portrait-portrait",
                        rotation: [l, u],
                        colors: [D, H],
                        btns: W,
                        dir: [1, 0],
                    },
                },
                p3: {
                    "portrait-portrait-portrait-portrait_v1": {
                        key: "portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u],
                        colors: [D, H, !1, B],
                        btns: W,
                        dir: [2, 0, 1],
                    },
                    "portrait-portrait-portrait-portrait_v2": {
                        key: "portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u],
                        colors: [D, H, B, !1],
                        btns: W,
                        dir: [2, 0, 1],
                    },
                    "portrait-portrait-landscape": {
                        key: "portrait-portrait-landscape",
                        rotation: [l, u, ae],
                        colors: [D, H, B],
                        btns: { single: ln, multi: Je },
                        dir: [2, 0, 1],
                    },
                    "portrait-portrait-portrait-portrait_v3": {
                        key: "portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u],
                        colors: [!1, D, H, B],
                        btns: W,
                        dir: [2, 1, 0],
                    },
                    "portrait-portrait-portrait-portrait_v4": {
                        key: "portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u],
                        colors: [D, !1, H, B],
                        btns: W,
                        dir: [2, 1, 0],
                    },
                    "landscape-portrait-portrait": {
                        key: "landscape-portrait-portrait",
                        rotation: [Fe, l, u],
                        colors: [D, H, B],
                        btns: { single: mt, multi: Je },
                        dir: [2, 1, 0],
                    },
                },
                p4: {
                    "landscape-portrait-portrait-landscape": {
                        key: "landscape-portrait-portrait-landscape",
                        rotation: [Fe, l, u, ae],
                        colors: [D, H, q, B],
                        btns: { single: ln, multi: Je },
                        dir: [3, 1, 0, 2],
                    },
                    "portrait-portrait-portrait-portrait": {
                        key: "portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u],
                        colors: [D, q, H, B],
                        btns: W,
                        dir: [3, 2, 0, 1],
                    },
                },
                p5: {
                    "portrait-portrait-portrait-portrait-landscape": {
                        key: "portrait-portrait-portrait-portrait-landscape",
                        rotation: [l, u, l, u, ae],
                        colors: [D, q, H, B, Q],
                        anchors: [3, 4],
                        btns: W,
                        dir: [4, 2, 0, 1, 3],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait_v1": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u],
                        colors: [D, q, H, B, !1, Q],
                        btns: W,
                        dir: [4, 2, 0, 1, 3],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait_v2": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, !1],
                        btns: W,
                        dir: [4, 2, 0, 1, 3],
                    },
                    "landscape-portrait-portrait-portrait-portrait": {
                        key: "landscape-portrait-portrait-portrait-portrait",
                        rotation: [Fe, l, u, l, u],
                        colors: [D, q, H, B, Q],
                        btns: W,
                        dir: [4, 3, 1, 0, 2],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait_v3": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u],
                        colors: [!1, D, q, H, B, Q],
                        btns: W,
                        dir: [4, 3, 1, 0, 2],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait_v4": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u],
                        colors: [D, !1, q, H, B, Q],
                        btns: W,
                        dir: [4, 3, 1, 0, 2],
                    },
                },
                p6: {
                    "portrait-portrait-portrait-portrait-portrait-portrait": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re],
                        btns: W,
                        dir: [5, 4, 2, 0, 1, 3],
                    },
                    "landscape-portrait-portrait-portrait-portrait-landscape": {
                        key: "landscape-portrait-portrait-portrait-portrait-landscape",
                        rotation: [Fe, l, u, l, u, ae],
                        colors: [D, q, H, B, Q, re],
                        anchors: [5, 4],
                        btns: W,
                        dir: [5, 3, 1, 0, 2, 4],
                    },
                },
                p7: {
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait_v1": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, !1, ke],
                        anchors: [5, 6],
                        btns: W,
                        dir: [6, 4, 2, 0, 1, 3, 5],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait_v2": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, ke, !1],
                        anchors: [5, 6],
                        btns: W,
                        dir: [6, 4, 2, 0, 1, 3, 5],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait-landscape": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-landscape",
                        rotation: [l, u, l, u, l, u, ae],
                        colors: [D, q, H, B, Q, re, ke],
                        anchors: [5, 6],
                        btns: W,
                        dir: [6, 4, 2, 0, 1, 3, 5],
                    },
                },
                p8: {
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, ke, nt],
                        anchors: [6, 7],
                        btns: W,
                        dir: [7, 6, 4, 2, 0, 1, 3, 5],
                    },
                    "landscape-portrait-portrait-portrait-portrait-portrait-portrait-landscape": {
                        key: "landscape-portrait-portrait-portrait-portrait-portrait-portrait-landscape",
                        rotation: [Fe, l, u, l, u, l, u, ae],
                        colors: [D, q, H, B, Q, re, ke, ts],
                        anchors: [6, 7],
                        btns: W,
                        dir: [7, 5, 3, 1, 0, 2, 4, 6],
                    },
                },
                p9: {
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait_v1": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, ke, nt, !1, Ft],
                        anchors: [7, 8],
                        btns: W,
                        dir: [8, 6, 4, 2, 0, 1, 3, 5, 7],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait_v2": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, ke, nt, Ft, !1],
                        anchors: [7, 8],
                        btns: W,
                        dir: [8, 6, 4, 2, 0, 1, 3, 5, 7],
                    },
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-landscape": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-landscape",
                        rotation: [l, u, l, u, l, u, l, u, ae],
                        colors: [D, q, H, B, Q, re, ke, ts, nt],
                        anchors: [7, 8],
                        btns: W,
                        dir: [8, 6, 4, 2, 0, 1, 3, 5, 7],
                    },
                },
                p10: {
                    "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait": {
                        key: "portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait",
                        rotation: [l, u, l, u, l, u, l, u, l, u],
                        colors: [D, q, H, B, Q, re, Un, nt, es, Ft],
                        anchors: [8, 9],
                        btns: { single: ln, multi: Fn },
                        dir: [9, 8, 6, 4, 2, 0, 1, 3, 5, 7],
                    },
                    "landscape-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-landscape": {
                        key: "landscape-portrait-portrait-portrait-portrait-portrait-portrait-portrait-portrait-landscape",
                        rotation: [Fe, l, u, l, u, l, u, l, u, ae],
                        colors: [D, q, H, B, Q, re, Un, nt, es, Ft],
                        anchors: [8, 9],
                        btns: { single: ln, multi: Fn },
                        dir: [9, 7, 5, 3, 1, 0, 2, 4, 6, 8],
                    },
                },
            },
            Pn = {
                down: "translateY(calc(100% - 44px))",
                up: "translateY(calc(-100% + 34px))",
                left: "translateX(calc(-100% - 5px))",
                right: "translateX(calc(100% + 5px))",
            },
            Bn = [],
            qi = () => {
                (G[`p${A}`] && G[`p${A}`][$]) ||
                    ((A = 4), ($ = "portrait-portrait-portrait-portrait"), (j = $n()), (It = 0), U());
            },
            oa = () => {
                j.forEach((e, t) => {
                    (e.background = G[`p${A}`][$].colors[t]), (e.backgroundImage = !1), (e.backgroundImagePartner = !1);
                }),
                    U();
            },
            ra = (e) => {
                const t = [...we].sort(() => Math.random() - 0.5);
                return t.slice(0, e);
            },
            $n = (e = !1) => {
                const t = [],
                    n = G[`p${A}`][$].colors.filter((e) => e !== !1),
                    s = a.randomPlayerColors ? ra(A) : n;
                for (let e = 0; e < A; e++)
                    t.push({
                        name: `Player ${e + 1}`,
                        nickname: "",
                        life: te,
                        background: s[e],
                        backgroundImage: !1,
                        backgroundImagePartner: !1,
                        alive: !0,
                        partnerCommander: !1,
                        commanderDamage: [],
                    });
                if (e) {
                    const n = e.reverse();
                    for (let e = 0; e < A; e++)
                        n[e] &&
                            ((t[e].partnerCommander = n[e].partnerCommander || t[e].partnerCommander),
                            (t[e].nickname = n[e].nickname || ""),
                            a.randomPlayerColors
                                ? a.preserveBackgroundImagesOnShuffle &&
                                  ((t[e].backgroundImage = n[e].backgroundImage || !1),
                                  (t[e].backgroundImagePartner = n[e].backgroundImagePartner || !1))
                                : ((t[e].background = n[e].background || t[e].background),
                                  (t[e].backgroundImage = n[e].backgroundImage || !1),
                                  (t[e].backgroundImagePartner = n[e].backgroundImagePartner || !1)));
                }
                return t.reverse();
            },
            fa = (e) => {
                g("--width", e.clientWidth + "px", e),
                    g("--height", e.clientHeight + "px", e),
                    g("--aspect-ratio-card", e.clientWidth / e.clientHeight, e),
                    Array.from(e.classList).some((e) => e.includes("rotate")) &&
                        g("--aspect-ratio-card", e.clientHeight / e.clientWidth, e);
            },
            vo = (e) => {
                const t = window.getComputedStyle(e);
                return t.transform || t.webkitTransform || t.mozTransform;
            },
            mo = (e, t) => {
                const n = vo(e),
                    s = n === Ge ? t : `${n} ${t}`;
                C(e, "transform", s);
            },
            dr = (e) => {
                n(e),
                    setTimeout(() => {
                        r(e);
                    }, 200);
            },
            co = (n, o = 100) => {
                const i = () => {
                    const o = e(["particle"]);
                    return s(o, "★"), no(o, n), t([o], n), o.addEventListener(x, () => no(o, n)), o;
                };
                requestAnimationFrame(() => {
                    Array.from({ length: o }).forEach(i);
                });
            },
            no = (e, t) => {
                const s = () => {
                        const n = Math.random() * t.clientWidth,
                            s = Math.random() * t.clientHeight;
                        C(e, Y, `${n}px`), C(e, "top", `${s}px`);
                    },
                    o = () => {
                        const t = Math.random() * 5 + 5;
                        C(e, "width", `${t}px`), C(e, "height", `${t}px`);
                    },
                    i = (t) => {
                        const n = Math.random() * t * 2;
                        C(e, "animationDelay", `-${n}s`);
                    },
                    a = () => {
                        const t = Math.random() * 360;
                        g("--rotation", `${t}deg`, e);
                    },
                    r = () => {
                        const t = Math.floor(Math.random() * we.length);
                        g("--color", `var(--${we[t]})`, e);
                    },
                    n = Math.random() * (1 - 0.2) + 0.2;
                s(),
                    o(),
                    i(n),
                    a(),
                    r(),
                    C(e, "animation", Ge),
                    e.offsetHeight,
                    C(
                        e,
                        "animation",
                        `growParticle ${n}s linear forwards, fadeInOut ${n}s linear forwards, fontColorChange ${n}s linear forwards`
                    );
            },
            c = (e, t) => localStorage.setItem(e, JSON.stringify(t)),
            b = (e) => JSON.parse(localStorage.getItem(e)),
            hs = "playerCount",
            Vt = "startingLife2P",
            Ht = "startingLifeMP",
            ls = "layoutType",
            Qt = "players",
            Os = "gameHistory",
            Qe = "planechase",
            _s = "planechaseDataV2",
            Bt = "planechaseEnabledCards",
            Ke = "archenemy",
            fs = "archenemyDataV2",
            Wt = "archenemyEnabledCards",
            Be = "bounty",
            us = "bountyDataV2",
            qt = "bountyEnabledCards",
            Ie = "turnTracker",
            L = "gameSettings",
            ze = "profiles",
            Es = () =>
                new Promise((e, t) => {
                    const n = indexedDB.open("lotusStore", 1);
                    (n.onupgradeneeded = (e) => e.target.result.createObjectStore("keyval")),
                        (n.onsuccess = (t) => e(t.target.result)),
                        (n.onerror = () => t(n.error));
                }),
            Is = (e) =>
                Es().then(
                    (t) =>
                        new Promise((n, s) => {
                            const o = t.transaction("keyval", "readonly").objectStore("keyval").get(e);
                            (o.onsuccess = () => n(o.result)), (o.onerror = () => s(o.error));
                        })
                ),
            fo = (e, t) =>
                Es().then(
                    (n) =>
                        new Promise((s, o) => {
                            const i = n.transaction("keyval", "readwrite");
                            i.objectStore("keyval").put(t, e),
                                (i.oncomplete = () => s(!0)),
                                (i.onerror = () => o(i.error)),
                                (i.onabort = () => o(i.error));
                        })
                );
        let bo = !1;
        const Ye = async () => {
                if (!bo) return;
                try {
                    await fo(ze, K);
                } catch (e) {
                    console.error("Profiles IndexedDB save failed, falling back to localStorage", e);
                    try {
                        localStorage.setItem(ze, JSON.stringify(K));
                    } catch {
                        _e("Could not save profile — storage is full", "error");
                    }
                }
            },
            Za = async () => {
                try {
                    let e;
                    try {
                        e = await Is(ze);
                    } catch (e) {
                        console.error("Profiles DB read failed; keeping localStorage copy", e);
                        return;
                    }
                    if (Array.isArray(e)) {
                        K = e.map(Xe);
                        return;
                    }
                    const t = b(ze);
                    if (Array.isArray(t) && t.length) {
                        K = t.map(Xe);
                        try {
                            await fo(ze, K);
                            const e = await Is(ze),
                                t = Array.isArray(e) && e.length === K.length;
                            t
                                ? localStorage.removeItem(ze)
                                : console.error("Profiles migration read-back mismatch; keeping localStorage copy");
                        } catch (e) {
                            console.error("Profiles migration failed; keeping localStorage copy", e);
                        }
                    }
                } finally {
                    bo = !0;
                }
            },
            he = "gameTimerState",
            Ro = "customDiceValue",
            In = "downloadBannerDismissed",
            Rn = "reviewBannerDismissed",
            ni = "patreonBannerDismissed",
            kn = "defeatMSGs",
            En = "victoryMSGs",
            xn = "commanderDefeatMSGs",
            _n = "poisonDefeatMSGs",
            vn = "recentPlayerNames",
            Nn = "hiddenPlayerNames",
            je = "allGamesHistory",
            at = "currentGameMeta",
            mn = "gameCounter",
            bn = () => Date.now().toString(36) + Math.random().toString(36).substring(2, 7),
            ai = (e) => {
                const t = new Date(e),
                    n = t.toLocaleString("en-US", { month: "short" }),
                    s = t.getDate().toString().padStart(2, "0"),
                    o = t.getHours().toString().padStart(2, "0"),
                    i = t.getMinutes().toString().padStart(2, "0");
                return `${n} ${s}, ${o}:${i}`;
            },
            yn = (e) => {
                if (!e || e === 0) return "00:00";
                const t = Math.floor(e / 3600),
                    n = Math.floor((e % 3600) / 60),
                    s = e % 60;
                return t > 0
                    ? `${t.toString().padStart(2, "0")}:${n.toString().padStart(2, "0")}:${s.toString().padStart(2, "0")}`
                    : `${n.toString().padStart(2, "0")}:${s.toString().padStart(2, "0")}`;
            },
            hn = () =>
                O && O.isActive
                    ? "bounty"
                    : y && y.isActive
                      ? "archenemy"
                      : k && k.isActive
                        ? "planechase"
                        : A > 2 && te >= 40
                          ? "commander"
                          : "standard",
            ba = () => {
                xe++;
                const e = {
                    id: bn(),
                    name: `Game #${xe}`,
                    startDate: Date.now(),
                    startingLife: te,
                    playerCount: A,
                    gameMode: hn(),
                };
                return (v = e), c(mn, xe), c(at, v), e;
            },
            oi = () => {
                if (!j || j.length === 0) return null;
                const e = j.filter((e) => e.alive);
                return e.length === 1 ? j.indexOf(e[0]) : null;
            },
            Mt = (e) => {
                const t = b(he);
                return t && t.startTime
                    ? t.isPaused
                        ? Math.floor((t.pausedTime - t.startTime) / 1e3)
                        : Math.floor((Date.now() - t.startTime) / 1e3)
                    : Math.floor((Date.now() - e) / 1e3);
            },
            cr = () => {
                if (!v) return;
                typeof He == "function" && He();
                const t = oi(),
                    n = j.map((e, n) => ({
                        name: e.name,
                        nickname: e.nickname || "",
                        life: e.life,
                        background: e.background,
                        backgroundImage: e.backgroundImage,
                        alive: e.alive,
                        isWinner: n === t,
                        commanderDamage: e.commanderDamage,
                    })),
                    s = v.gameModes || [],
                    o = s.filter((e) => {
                        const t = [
                            "commander",
                            "dual-commander",
                            "standard",
                            "modern",
                            "pauper",
                            "legacy",
                            "archon",
                            "australian-highlander",
                            "canadian-highlander",
                            "centurion",
                            "conquest",
                            "co",
                            "dandan",
                            "european-highlander",
                            "leviathan",
                            "oathbreaker",
                            "old-school",
                            "pauper-commander",
                            "pioneer",
                            "predh",
                            "premodern",
                            "primordial",
                            "tiny-leaders",
                            "vintage",
                        ];
                        return t.includes(e);
                    }),
                    e = [];
                k && k.isActive && e.push("planechase"),
                    y && y.isActive && e.push("archenemy"),
                    O && O.isActive && e.push("bounty");
                const i = [...o, ...e],
                    a = h && h.isActive && h.ongoingGame ? h.currentTurn : null,
                    r = {
                        id: v.id,
                        name: v.name,
                        startDate: v.startDate,
                        endDate: Date.now(),
                        duration: Mt(v.startDate),
                        startingLife: v.startingLife,
                        playerCount: v.playerCount,
                        gameMode: v.gameMode,
                        gameModes: i,
                        turnCount: a,
                        players: n,
                        history: [...Re],
                        winner: t,
                    };
                R.unshift(r), c(je, R);
            },
            U = () => {
                c(hs, A),
                    c(Vt, Se),
                    c(Ht, Ue),
                    c(ls, $),
                    c(Qt, j),
                    c(Os, Re),
                    c(Qe, k),
                    c(Ke, y),
                    c(Ie, h),
                    c(L, a),
                    c(je, R),
                    c(at, v),
                    c(mn, xe);
            },
            Ct = (e) => e.toString().replace("life", "").trim(),
            cn = () => {
                Number(A) === 2 ? (te = Ct(Se)) : (te = Ct(Ue)), (te = Number(te));
            },
            rr = {
                autoKO: !0,
                lifeLossOnCommanderDamage: !0,
                showCountersOnPlayerCard: !0,
                showRegularCounters: !0,
                showCommanderDamageCounters: !1,
                clickableCommanderDamageCounters: !1,
                keepZeroCountersOnPlayerCard: !1,
                whitelabelIcon: null,
                saltyDefeatMessages: !0,
                cycleSaltyDefeatMessages: !0,
                gameTimer: !1,
                gameTimerMainScreen: !1,
                showClockOnMainScreen: !1,
                clockFormat24h: !0,
                randomPlayerColors: !1,
                preserveBackgroundImagesOnShuffle: !0,
                setLifeByTappingNumber: !0,
                verticalTapAreas: !1,
                cleanLook: !1,
                barredDigits: !1,
                criticalDamageWarning: !0,
                customLongTapEnabled: !1,
                customLongTapValue: 10,
                customSingleTapEnabled: !1,
                customSingleTapValue: 1,
                showPlayerNamesOnCard: !0,
                persistentCounters: [],
                pinnedDie: null,
            },
            sr = () => {
                (A = Number(b(hs)) || 4),
                    (Se = b(Vt) || 20),
                    (Ue = b(Ht) || 40),
                    ($ = b(ls) || "portrait-portrait-portrait-portrait"),
                    (Re = b(Os) || []),
                    (k = b(Qe) || { isActive: !1, includeBeyondUniverse: !1, history: [], currentPlane: null }),
                    (y = b(Ke) || { isActive: !1, history: [], ongoingSchemes: [], currentScheme: null }),
                    (O = b(Be) || { isActive: !1, history: [], currentBounty: null, rewardLevel: 1 }),
                    (h = b(Ie) || {
                        isActive: !1,
                        ongoingGame: !1,
                        autoHighroll: !1,
                        turnTimer: { isActive: !1, duration: 0, countDown: [] },
                        currentPlayerIndex: 0,
                        startingPlayerIndex: null,
                        currentTurn: 1,
                    }),
                    (a = { ...rr, ...b(L) }),
                    (K = (b(ze) || []).map(Xe)),
                    Za(),
                    cn(),
                    qi(),
                    (j = b(Qt) || $n()),
                    (R = b(je) || []),
                    (xe = b(mn) || 0),
                    (v = b(at)),
                    v ||
                        (j && j.length > 0
                            ? (xe++,
                              (v = {
                                  id: bn(),
                                  name: `Game #${xe}`,
                                  startDate: Date.now(),
                                  startingLife: te,
                                  playerCount: A,
                                  gameMode: hn(),
                              }))
                            : (xe++,
                              (v = {
                                  id: bn(),
                                  name: `Game #${xe}`,
                                  startDate: Date.now(),
                                  startingLife: te,
                                  playerCount: A,
                                  gameMode: hn(),
                              })),
                        c(mn, xe),
                        c(at, v),
                        c(je, R));
            },
            Xo = () => {
                a.verticalTapAreas || ($ === "landscape" && A === 1)
                    ? n(d, "vertical-tap-areas")
                    : r(d, "vertical-tap-areas");
            },
            Go = () => {
                a.cleanLook ? n(d, "clean-look") : r(d, "clean-look");
            },
            Vo = () => {
                a.barredDigits ? n(d, "barred-digits") : r(d, "barred-digits");
            },
            Dn = [40, 30, 20, "custom"],
            Io = {
                startingLifeMP: { label: "Multi-player starting life", options: Dn },
                startingLife2P: { label: "Two-player starting life", options: Dn },
            },
            Ho = (e, t, n, s = !1) => {
                if (s === "commander" && !a.lifeLossOnCommanderDamage) {
                    U();
                    return;
                }
                const o = n.life;
                (n.life += e),
                    S(t, n.life),
                    Bo(n, e),
                    U(),
                    ut(t, "big", n.life.toString().length === 3),
                    a.autoKO && o > 0 && n.life <= 0 && ft(n, "life"),
                    it(n);
            },
            it = (t) => {
                if (te === 0 || !a.criticalDamageWarning) {
                    const n = t.playerCard.children[0],
                        e = n.querySelector(".critical-damage-overlay");
                    e && e.remove();
                    return;
                }
                const n = t.playerCard.children[0],
                    s = n.querySelector(".critical-damage-overlay");
                if (t.life < 10 && t.life > 0 && t.alive) {
                    if (!s) {
                        const t = e(["critical-damage-overlay"]);
                        n.insertBefore(t, n.firstChild);
                    }
                } else s && s.remove();
            },
            Po = () => j.some((e) => e.playerCard?.children?.[0]?.classList.contains("dragging")),
            Hn = () => j.some((e) => e.playerCard?.children?.[0]?.classList.contains("commander-damage-active")),
            ht = (e) => {
                const t = m.querySelector(".monarch-coin"),
                    s = m.querySelector(".initiative-coin");
                if ((e === Z && (mo(z, "scale(0.1)"), n(z, Z), n(ge, Z), t && n(t, Z), s && n(s, Z)), e === Et)) {
                    const e = j.some((e) =>
                        Array.from(e.playerCard.children[0].classList).includes("commander-damage-active")
                    );
                    if (e) return;
                    C(z, "transform", ""), r(z, Z), t && r(t, Z), s && r(s, Z), r(ge, Z);
                }
            },
            ft = (e, s = "life") => {
                if (!a.autoKO) return;
                He(), (e.alive = !1), Hs();
                const i = e.playerCard.children,
                    o = i[0],
                    l = o.querySelector(".killed-overlay"),
                    r = o.querySelector(".critical-damage-overlay"),
                    c = i?.[1]?.children?.[0] ?? null;
                r && r.remove(),
                    e.playerCardInnerClick(),
                    n(o, be),
                    c && c.click(),
                    l || t([Jt(be, e, s)], o),
                    _(z, Z) && ht(Et),
                    U(),
                    Zt();
            },
            Lo = (n, s, o = 1) => {
                const d = e(["commander-damage-card-inner"]),
                    u = s.playerCard.children[0].querySelector(".player-life-count");
                let i = s.commanderDamage.find((e) => e.player === n.name);
                i
                    ? i.damage[`commander${o}`] || (i.damage[`commander${o}`] = 0)
                    : ((i = { player: n.name, damage: { [`commander${o}`]: 0 } }), s.commanderDamage.push(i));
                const r = e(["damage-display"]),
                    c = e(["increase-button", "commander-damage"]),
                    l = e(["decrease-button", "commander-damage"]),
                    a = { increase: e(["life-change-feedback"]), decrease: e(["life-change-feedback"]) };
                S(a.increase, "+"),
                    S(a.decrease, "-"),
                    t([a.increase], c),
                    t([a.decrease], l),
                    (r.innerText = `Damage: ${i.damage[`commander${o}`]}`),
                    ot(r);
                const h = (e, t, n, o, a) => {
                    i &&
                        ((i.damage[`commander${a}`] += e),
                        Ho(-e, u, s, "commander"),
                        i.damage[`commander${a}`] >= 21 && ft(s, "commander"),
                        S(t, i.damage[`commander${a}`]),
                        me(s));
                };
                return yo(n, r, c, l, a, h, "commanderDamage", o, s), t([r, c, l], d), d;
            },
            Mo = "ownCommanderDamageHintOverlay_v1",
            Wn = () => b(Mo) === !0,
            tr = () => c(Mo, !0),
            Eo = () => {
                if (Wn()) return;
                const a = e(["own-commander-damage-hint-overlay"]),
                    c = e([N]),
                    l = e([p]);
                c.addEventListener(o, () => {
                    n(a, E),
                        a.addEventListener(
                            x,
                            () => {
                                a.remove();
                                const e = m.querySelector(".other-buttons-wrapper");
                                e && r(e, "commander-dmg-overlay-active");
                            },
                            w
                        ),
                        tr(),
                        i("Close Own Commander Damage Hint Overlay");
                }),
                    (l.innerHTML +=
                        "Got betrayed?<br><span>Tap the dagger to track damage taken from your own commander.</span>"),
                    s(c, "Got it!"),
                    t([l, c], a),
                    t([a], d);
                const u = m.querySelector(".other-buttons-wrapper");
                u && n(u, "commander-dmg-overlay-active"), i("Own Commander Damage Hint Overlay", F);
            },
            xo = (s, i, a = !1) => {
                const c = s === i ? s.playerCard.lastElementChild : s.playerCard.children[0],
                    r = e(["commander-damage-card"]),
                    l = Lo(s, i);
                if ((n(c, "commander-damage-active"), t([l], r), t([r], c), s.partnerCommander)) {
                    const e = Lo(s, i, 2);
                    t([e], r), n(r, "partner-commander");
                }
                s === i &&
                    a &&
                    (n(c, "own-damage"),
                    n(a, "close"),
                    a.addEventListener(o, () => {
                        ti(),
                            n(a, Z),
                            a.addEventListener(oe, () => {
                                a.remove();
                            });
                    }));
            },
            Ja = (a) => {
                m.querySelectorAll(".switch-to-own-damage").forEach((e) => e.remove());
                const h = a.playerCard.children[0],
                    u = Rt(a, "info-card"),
                    l = e(["info-card-inner"]),
                    f = e(["info-card-text"]),
                    p = e(["return-to-game-button"]),
                    c = e(["switch-to-own-damage"]),
                    g = () => {
                        n(d, Ee),
                            a.playerCardInnerClick(),
                            v.forEach((e) => {
                                const t = e.playerCard.children[0],
                                    s = e.playerCard.querySelectorAll(".commander-damage-card");
                                r(t, "commander-damage-active"),
                                    s.forEach((e) => {
                                        n(e, Z),
                                            e.addEventListener(oe, () => {
                                                e.remove();
                                            });
                                    });
                            });
                        const e = () => {
                            u.remove(), h.removeEventListener(oe, e), r(d, Ee);
                        };
                        h.addEventListener(oe, e),
                            ht(Et),
                            n(c, Z),
                            c.addEventListener(oe, () => {
                                c.remove();
                            });
                    },
                    v = j.filter((e) => e !== a);
                v.forEach((e) => {
                    e.playerCardInnerClick(), xo(e, a);
                }),
                    li.forEach((e) => {
                        l.addEventListener(e, g);
                    }),
                    (f.innerHTML = `<b>Commander</b>Damage you’ve received`),
                    s(p, "Return to Game");
                let b = !1;
                c.addEventListener(o, () => {
                    if (!Wn()) {
                        Eo();
                        return;
                    }
                    b || (xo(a, a, c), (b = !0));
                }),
                    ht(Z),
                    t([f, p], l),
                    t([l], u),
                    t([u], a.playerCard),
                    t([c], ge.parentNode),
                    Jo(l),
                    Oo(l, "--font-size-commander"),
                    (ti = g),
                    It++,
                    It === 2 && !Wn() && Eo(),
                    i("Commander Damage Card", "Swipe", "Player Card");
            },
            Oo = (e, t) => {
                const s = (e, t) => parseFloat(getComputedStyle(t).getPropertyValue(e)),
                    o = () => e.scrollHeight > e.clientHeight;
                let n = s(t, e);
                for (; o() && n > 0; ) n--, g(t, `${n * 0.8}px`, e);
            },
            wt = (e) => {
                for (let t = e.length - 1; t > 0; t--) {
                    const n = Math.floor(Math.random() * (t + 1));
                    [e[t], e[n]] = [e[n], e[t]];
                }
            },
            Le = (e = 8e3, t = 12e3) => Math.floor(Math.random() * (t - e + 1)) + e,
            po = (e, t) => {
                const n = [...e].sort(),
                    s = [...t].sort();
                return n.toString() === s.toString();
            },
            Jt = (n, o = !1, c = "life") => {
                const f = "Defeated",
                    x = "Victory",
                    O = n === lt ? "winner-overlay" : "killed-overlay",
                    h = e([O]),
                    u = e([p]),
                    _ = () => {
                        if (c === "poison") {
                            {
                                const t = b(_n),
                                    e = Array.isArray(t) ? t.filter((e) => e.a && e.m).map((e) => e.m) : [...lo];
                                if (e.length > 0) return e[Math.floor(Math.random() * e.length)];
                            }
                        } else if (c === "commander") {
                            const t = b(xn),
                                e = Array.isArray(t) ? t.filter((e) => e.a && e.m).map((e) => e.m) : [...Ss];
                            if (e.length > 0) return e[Math.floor(Math.random() * e.length)];
                        }
                        return null;
                    },
                    g = () => {
                        const e = b(kn);
                        if (!Array.isArray(e)) return [...eo];
                        const t = e.filter((e) => e.a && e.m).map((e) => e.m);
                        return t.length > 0 ? t : [f];
                    },
                    v = () => {
                        const e = b(En);
                        if (!Array.isArray(e)) return [...Vs];
                        const t = e.filter((e) => e.a && e.m).map((e) => e.m);
                        return t.length > 0 ? t : [x];
                    },
                    j = () => (m >= l.length && (wt(l), (m = 0)), l[m++]),
                    w = () => {
                        if (n === be && !a.saltyDefeatMessages) {
                            s(u, f);
                            return;
                        }
                        if (n === be && !a.cycleSaltyDefeatMessages) return;
                        if (n !== lt) {
                            const e = g();
                            po(l, e) || ((l = [...e]), wt(l));
                        } else {
                            const e = v();
                            po(l, e) || ((l = [...e]), wt(l));
                        }
                        C(u, "opacity", 0),
                            setTimeout(() => {
                                s(u, j()), oo(u, h), C(u, "opacity", 1);
                            }, 500);
                    };
                let l = n === lt ? v() : g(),
                    m = 0,
                    E = Le();
                const y = _();
                return (
                    t([u], h),
                    wt(l),
                    n === be && !a.saltyDefeatMessages
                        ? s(u, f)
                        : y
                          ? (s(u, y), a.cycleSaltyDefeatMessages && (m = 0))
                          : s(u, j()),
                    setInterval(w, E),
                    setTimeout(() => {
                        oo(u, h);
                    }, 100),
                    o &&
                        setTimeout(() => {
                            new Hammer(h).on("tap", () => {
                                dt("Do you want to revive the killed player?", "Cancel", "Revive").then((e) => {
                                    if (e) {
                                        const e = o.playerCard.children[0],
                                            t = d.querySelector(".confirm-overlay");
                                        (o.alive = !0),
                                            e.querySelector(".killed-overlay").remove(),
                                            r(e, be),
                                            it(o),
                                            U(),
                                            Zt(),
                                            i("Unkill Player"),
                                            P(t);
                                    }
                                });
                            });
                        }, 1e3),
                    h
                );
            },
            oo = (n, o) => {
                const r = 100,
                    i = e();
                C(i, "position", "absolute"),
                    C(i, "visibility", "hidden"),
                    C(i, "width", `${o.clientWidth}px`),
                    C(i, "height", `${o.clientHeight}px`),
                    C(i, "fontSize", "1px"),
                    s(i, n.innerText),
                    t([i], d);
                let a = 1;
                for (; i.scrollWidth <= o.clientWidth && i.scrollHeight <= o.clientHeight && a < r; )
                    a++, C(i, "fontSize", `${a}px`);
                a--, a < 18 && (a = 18), C(n, "fontSize", `${a * 0.8}px`), i.remove();
            },
            Qa = () => {
                const e = b(he);
                if (!e || e.isPaused) return;
                (e.isPaused = !0),
                    (e.pausedTime = Date.now()),
                    (e.endedByWin = !0),
                    c(he, e),
                    (T.isPaused = !0),
                    (T.pausedTime = e.pausedTime),
                    T.node && (n(T.node, "paused"), n(T.node, "ended"));
            },
            Xa = () => {
                const e = b(he);
                if (!e || !e.endedByWin) return;
                e.isPaused && ((e.startTime += Date.now() - e.pausedTime), (e.isPaused = !1), (e.pausedTime = 0)),
                    (e.endedByWin = !1),
                    c(he, e),
                    (T.isPaused = !1),
                    (T.pausedTime = 0),
                    T.node && (r(T.node, "paused"), r(T.node, "ended"));
            },
            Zt = () => {
                if (j.length === 1) return;
                const e = j.filter((e) => e.alive);
                if (e.length === 1) {
                    const s = e[0].playerCard.children[0],
                        o = Jt(lt);
                    t([o], s), n(s, lt), co(o), Qa();
                } else
                    m.querySelectorAll(".winner").forEach((e) => {
                        r(e, lt), e.querySelectorAll(".winner-overlay").forEach((e) => e.remove());
                    }),
                        Xa();
            },
            Ga = (e) => {
                const n = e.split(","),
                    t = atob(n[1]),
                    o = n[0].split(":")[1].split(";")[0],
                    s = new ArrayBuffer(t.length),
                    i = new Uint8Array(s);
                for (let e = 0; e < t.length; e++) i[e] = t.charCodeAt(e);
                return new Blob([s], { type: o });
            },
            Ya = (e) =>
                new Promise((t, n) => {
                    const s = indexedDB.open("imageDatabase", 1);
                    (s.onsuccess = (s) => {
                        const a = s.target.result,
                            r = a.transaction(["images"], "readonly"),
                            c = r.objectStore("images"),
                            o = Number(e.split("/").pop());
                        if (isNaN(o)) {
                            n("Invalid key");
                            return;
                        }
                        const i = c.get(o);
                        (i.onsuccess = (e) => {
                            const s = e.target.result?.imageData;
                            if (s) {
                                const e = Ga(s);
                                t(URL.createObjectURL(e));
                            } else n("Image not found");
                        }),
                            (i.onerror = (e) => {
                                n("Error retrieving image:", e.target.error);
                            });
                    }),
                        (s.onerror = (e) => {
                            n("Database error:", e.target.errorCode);
                        });
                }),
            qa = (n, i, a) => {
                const j = e(["image-reposition-overlay"]),
                    M = e(["reposition-container"]),
                    r = e(["reposition-viewport"]),
                    c = e([], "img"),
                    T = e(["life-count-overlay"]),
                    z = e(["instruction-text"]),
                    E = e([N, "confirm"]),
                    k = e([N, "cancel"]),
                    L = e(["button-wrapper"]);
                let u = 1,
                    l = 1,
                    g = 0,
                    p = 0,
                    y = !1,
                    D = 0,
                    A = 0;
                (c.src = n),
                    (c.draggable = !1),
                    s(E, "Confirm"),
                    s(k, "Cancel"),
                    s(z, "Pinch to zoom • Drag to reposition"),
                    S(T, a?.life || 40),
                    (c.onload = () => {
                        requestAnimationFrame(() => {
                            const e = c.naturalWidth / c.naturalHeight,
                                t = 3 / 2,
                                n = r.clientWidth || r.offsetWidth,
                                s = r.clientHeight || r.offsetHeight;
                            e > t ? (l = s / c.naturalHeight) : (l = n / c.naturalWidth),
                                (!l || l <= 0 || !isFinite(l)) && (l = 1),
                                (u = l),
                                C();
                        });
                    });
                const C = () => {
                        c.style.transform = `translate(calc(-50% + ${g}px), calc(-50% + ${p}px)) scale(${u})`;
                    },
                    b = (e) => {
                        y = !0;
                        const t = e.touches ? e.touches[0].clientX : e.clientX,
                            n = e.touches ? e.touches[0].clientY : e.clientY;
                        (D = t - g), (A = n - p), (r.style.cursor = "grabbing");
                    },
                    v = (e) => {
                        if (!y) return;
                        e.preventDefault();
                        const t = e.touches ? e.touches[0].clientX : e.clientX,
                            n = e.touches ? e.touches[0].clientY : e.clientY;
                        (g = t - D), (p = n - A), C();
                    },
                    h = () => {
                        (y = !1), (r.style.cursor = "grab");
                    },
                    f = new Hammer(r);
                f.get("pinch").set({ enable: !0 });
                let F = 1,
                    _ = 0,
                    x = 0;
                f.on("pinchstart", (e) => {
                    F = u;
                    const t = r.getBoundingClientRect();
                    (_ = e.center.x - t.left - t.width / 2), (x = e.center.y - t.top - t.height / 2);
                }),
                    f.on("pinchmove", (e) => {
                        const t = F * e.scale;
                        if (u > 0) {
                            const e = t / u;
                            (g = _ + (g - _) * e), (p = x + (p - x) * e);
                        }
                        (u = t), C();
                    });
                const O = (e) => {
                        e.touches.length === 1 && b(e);
                    },
                    w = (e) => {
                        e.touches.length === 1 && v(e);
                    };
                r.addEventListener("mousedown", b),
                    m.addEventListener("mousemove", v),
                    m.addEventListener("mouseup", h),
                    r.addEventListener(st, O, { passive: !1 }),
                    m.addEventListener(_t, w, { passive: !1 }),
                    m.addEventListener(qe, h),
                    E.addEventListener(o, () => {
                        const e = m.createElement("canvas"),
                            g = e.getContext("2d"),
                            u = 1800,
                            p = 1200;
                        (e.width = u), (e.height = p);
                        const t = r.getBoundingClientRect(),
                            a = c.getBoundingClientRect(),
                            n = c.naturalWidth / a.width;
                        let s = (t.left - a.left) * n,
                            o = (t.top - a.top) * n,
                            l = t.width * n,
                            d = t.height * n;
                        (s = Math.max(0, Math.min(s, c.naturalWidth))),
                            (o = Math.max(0, Math.min(o, c.naturalHeight))),
                            (l = Math.min(l, c.naturalWidth - s)),
                            (d = Math.min(d, c.naturalHeight - o)),
                            g.drawImage(c, s, o, l, d, 0, 0, u, p);
                        const y = e.toDataURL("image/jpeg", 0.92);
                        f.destroy(),
                            r.removeEventListener("mousedown", b),
                            r.removeEventListener(st, O),
                            m.removeEventListener("mousemove", v),
                            m.removeEventListener("mouseup", h),
                            m.removeEventListener(_t, w),
                            m.removeEventListener(qe, h),
                            P(j, () => {
                                i(y);
                            });
                    }),
                    k.addEventListener(o, () => {
                        f.destroy(),
                            r.removeEventListener("mousedown", b),
                            r.removeEventListener(st, O),
                            m.removeEventListener("mousemove", v),
                            m.removeEventListener("mouseup", h),
                            m.removeEventListener(_t, w),
                            m.removeEventListener(qe, h),
                            P(j, () => {
                                i(null);
                            });
                    }),
                    t([c, T], r),
                    t([k, E], L),
                    t([z, r, L], M),
                    t([M], j),
                    t([j], d),
                    (r.style.cursor = "grab");
            },
            ks = (e, t, n = null) => {
                (e.type = "file"),
                    (e.accept = "image/*"),
                    e.addEventListener("change", (s) => {
                        const o = s.target.files[0];
                        if (o) {
                            const s = new FileReader();
                            (s.onload = (s) => {
                                const o = s.target.result;
                                qa(
                                    o,
                                    (n) => {
                                        n &&
                                            saveImage(n, (e) => {
                                                t(e);
                                            }),
                                            (e.value = "");
                                    },
                                    n
                                );
                            }),
                                s.readAsDataURL(o);
                        }
                    });
            },
            se = async (e) => (e ? (e.startsWith("index") && (e = await Ya(e)), e) : null),
            js = async (e, t, s, o = s) => {
                const i = await se(e),
                    a = await se(t);
                i && (g("--bgImage", `var(--gradientOverlay) ,url(${i})`, s), n(o, "background-image")),
                    a
                        ? (g("--bgImagePartner", `var(--gradientOverlay) ,url(${a})`, s),
                          n(o, "background-image-partner"))
                        : r(o, "background-image-partner");
            },
            Tt = async (e, t = e.playerCard, n = e.playerCard.children[0]) => {
                js(e.backgroundImage, e.backgroundImagePartner, t, n);
            },
            Ka = () => {
                const e = indexedDB.open("imageDatabase", 1);
                (e.onupgradeneeded = (e) => {
                    const t = e.target.result;
                    t.createObjectStore("images", { keyPath: "id", autoIncrement: !0 });
                }),
                    (e.onsuccess = (e) => {
                        const t = e.target.result;
                        window.saveImage = (e, n) => {
                            const s = t.transaction(["images"], "readwrite"),
                                o = s.objectStore("images"),
                                i = o.add({ imageData: e });
                            i.onsuccess = (e) => {
                                const t = e.target.result,
                                    s = `indexeddb://imageDatabase/images/${t}`;
                                n(s);
                            };
                        };
                    }),
                    (e.onerror = (e) => {
                        console.error("Database error:", e.target.errorCode);
                    });
            },
            Wa = (e) => {
                const t = indexedDB.open("imageDatabase", 1);
                t.onsuccess = (t) => {
                    const s = t.target.result,
                        o = s.transaction(["images"], "readwrite"),
                        i = o.objectStore("images"),
                        n = i.delete(e);
                    (n.onsuccess = () => {
                        console.log(`Reference to ${e} deleted from IndexedDB`);
                    }),
                        (n.onerror = () => {
                            console.error(`Failed to delete reference to ${e} from IndexedDB`);
                        });
                };
            },
            P = (e, t = !1) => {
                n(e, E),
                    e.addEventListener(x, () => {
                        e.remove(), t && t();
                    });
            },
            ps = (a, c, l = !1) => {
                i("Change Background Image");
                let f = 0,
                    v,
                    b = !1;
                const D = c.parentElement,
                    E = getComputedStyle(D).getPropertyValue("--aspect-ratio-card"),
                    u = e(["card-search-overlay"]),
                    h = e(["results-wrapper"]),
                    m = e(["search-input"], M),
                    O = e(["upload-input"], M),
                    _ = e([tt], "label"),
                    x = e(["close-card-search-overlay"]),
                    y = e(["giphy-switch"]),
                    T = ["Cards", "Giphy"],
                    z = (e, t) => {
                        let n;
                        return function (...s) {
                            clearTimeout(n),
                                (n = setTimeout(() => {
                                    e.apply(this, s);
                                }, t));
                        };
                    },
                    k = (n, r, c) => {
                        const m = e(["card-image"]),
                            d = e(["card-meta"]),
                            p = e(["artist-label"]),
                            f = e(["card-name"]),
                            g = e(["card-name-and-artist"]);
                        s(p, r.artist),
                            s(f, r.name),
                            C(m, "backgroundImage", `url(${n})`),
                            t([f, p], g),
                            t([g], d),
                            t([m, d], h),
                            r.artist === "" ? d.remove() : null,
                            c
                                ? fetch(c)
                                      .then((e) => e.json())
                                      .then((n) => {
                                          if (n.total_cards > 1) {
                                              const n = e(["view-all-prints-btn"]);
                                              s(n, "View all prints"),
                                                  n.addEventListener(o, (e) => {
                                                      e.stopPropagation(), h.scrollTo(0, 0), N(c);
                                                  }),
                                                  t([n], d);
                                          }
                                      })
                                : f.remove(),
                            m.addEventListener(o, () => {
                                l
                                    ? ((a.backgroundImagePartner = n), i("Set Partner Background Image"))
                                    : ((a.backgroundImage = n), i("Set Background Image")),
                                    Tt(a),
                                    U(),
                                    j.forEach((e) => me(e)),
                                    a.playerCardInnerClick(),
                                    P(u);
                            });
                    },
                    A = (n, o) => {
                        let i;
                        if (Array.isArray(n)) i = n;
                        else if (n && Array.isArray(n.data)) i = n.data;
                        else {
                            const n = e(["no-results"]);
                            s(n, $t[Le(0, $t.length - 1)]), t([n], h);
                            return;
                        }
                        const a = i.slice(f, f + 5);
                        if (
                            (a.forEach((e) => {
                                const t = !o && e.prints_search_uri;
                                e.image_uris && e.image_uris.art_crop
                                    ? k(e.image_uris.art_crop, e, t)
                                    : e.card_faces &&
                                      Array.isArray(e.card_faces) &&
                                      e.card_faces.forEach((n) => {
                                          n.image_uris && n.image_uris.art_crop && k(n.image_uris.art_crop, e, t);
                                      });
                            }),
                            (f += 5),
                            f < i.length)
                        ) {
                            const e = h.lastElementChild;
                            e && v.observe(e);
                        }
                    },
                    w = (e, t = !1) => {
                        ot(h),
                            (f = 0),
                            v && v.disconnect(),
                            (v = new IntersectionObserver(
                                (n) => {
                                    n[0].isIntersecting && (v.unobserve(n[0].target), A(e, t));
                                },
                                { root: h, rootMargin: "0px", threshold: 1 }
                            )),
                            A(e, t);
                    },
                    N = (e) => {
                        fetch(e)
                            .then((e) => e.json())
                            .then((e) => {
                                w(e, !0);
                            });
                    },
                    S = () => {
                        const e = m.value.trim();
                        if (e.length < 3) return;
                        r(_),
                            b
                                ? fetch(
                                      `https://api.giphy.com/v1/gifs/search?api_key=FHyo6eLOdvPX9qEWIhkisfeUvoXDXbG7&q=${e}&limit=25&offset=0&rating=g&lang=en`
                                  )
                                      .then((e) => e.json())
                                      .then((e) => {
                                          w(
                                              e.data.map((e) => ({
                                                  image_uris: { art_crop: e.images.fixed_height.url },
                                                  artist: e.username,
                                                  name: e.title,
                                                  prints_search_uri: null,
                                              }))
                                          ),
                                              n(u, "active-results");
                                      })
                                : fetch(`${dn}order=released&q=${e}`)
                                      .then((e) => e.json())
                                      .then((e) => {
                                          w(e), n(u, "active-results");
                                      });
                    },
                    F = () => {
                        m.placeholder = b ? "Search for a gif" : "Search for a card";
                    };
                F(),
                    (m.type = p),
                    (m.autocomplete = "off"),
                    m.addEventListener(M, z(S, 500)),
                    (O.id = "uploadInput"),
                    _.setAttribute("for", "uploadInput"),
                    (_.innerHTML = "<div>Upload Image</div><span>or</span>"),
                    ks(
                        O,
                        (e) => {
                            e &&
                                (l
                                    ? ((a.backgroundImagePartner = e), i("Set Partner Background Image"))
                                    : ((a.backgroundImage = e), i("Set Background Image")),
                                Tt(a),
                                U(),
                                j.forEach((e) => me(e)),
                                a.playerCardInnerClick(),
                                P(u));
                        },
                        a
                    ),
                    x.addEventListener(o, () => P(u)),
                    y.addEventListener(o, () => {
                        (b = !b), ut(y, tt, b), F(), m.value.length > 2 && S();
                    }),
                    T.forEach((n) => {
                        const o = e(["giphy-switch-label"]);
                        s(o, n), t([o], y);
                    }),
                    E && g("--aspect-ratio-card", E, u),
                    t([h, m, O, _, x, y], u),
                    t([u], d);
            },
            gn = (e) => {
                e = e.replace(/^#/, "");
                let t = parseInt(e.substr(0, 2), 16),
                    n = parseInt(e.substr(2, 2), 16),
                    s = parseInt(e.substr(4, 2), 16),
                    o = (t * 299 + n * 587 + s * 114) / 1e3;
                return o >= 128;
            },
            vs = (e, t, s = t) => {
                const o = !we.includes(e);
                g("--bg", o ? e : `var(--${e})`, t), r(s, "white-text"), o && (gn(e) || n(s, "white-text"));
            },
            bs = (e) => {
                const t = e.playerCard,
                    n = t.children[0];
                vs(e.background, t, n);
            },
            ds = (e, t, n) => {
                r(n, "background-image"),
                    (t.backgroundImage = !1),
                    (t.backgroundImagePartner = !1),
                    (t.background = e),
                    U(),
                    bs(t);
            },
            Va = (s, o, a) => {
                const c = e(["color-entry", "color-picker-entry"]),
                    l = e([], M),
                    d = e([], "label"),
                    u = e(["divider"]);
                we.includes(s.background) ? r(c, "color-set") : n(c, "color-set"),
                    (l.type = "color"),
                    (l.id = `color-${s.name}`),
                    l.addEventListener(M, (e) => {
                        ds(e.target.value, s, a),
                            i("Set Custom Color Background"),
                            n(c, "color-set"),
                            j.forEach((e) => me(e));
                    }),
                    d.setAttribute("for", `color-${s.name}`),
                    t([l, d], c),
                    t([c, u], o);
            },
            et = [
                "https://cards.scryfall.io/art_crop/front/d/0/d0d33d52-3d28-4635-b985-51e126289259.jpg?1599707796",
                "https://cards.scryfall.io/art_crop/front/e/4/e4b1aa1e-b4e3-4346-8937-76b312501c70.jpg?1673307974",
                "https://cards.scryfall.io/art_crop/front/4/d/4dc94b73-ce6a-4360-b2bd-732f6857ddd5.jpg?1693659533",
                "https://cards.scryfall.io/art_crop/front/1/0/10d42b35-844f-4a64-9981-c6118d45e826.jpg?1689999317",
                "https://cards.scryfall.io/art_crop/front/0/e/0e259db1-14db-4314-998c-6a076a28d8cb.jpg?1691056044",
                "https://cards.scryfall.io/art_crop/front/7/d/7d00007d-6878-44e9-ab0d-396c25655e56.jpg?1702429511",
                "https://cards.scryfall.io/art_crop/front/f/2/f26eeb39-77ab-4e60-8b10-1420e755c372.jpg?1647735095",
                "https://cards.scryfall.io/art_crop/front/f/0/f058f8e8-aa01-4dc0-a6a5-0490521b83d4.jpg?1690001729",
                "https://cards.scryfall.io/art_crop/front/2/1/21eaf326-5586-4ff2-9d86-df49a0a62cca.jpg?1641685040",
            ],
            Pa = async (s, i, a) => {
                let h = 1,
                    c = 2;
                const r = e(["color-entry", "image-entry"]),
                    m = e(["image-entry-icon"]),
                    p = e(["divider"]),
                    v = () => {
                        _(r, "crossfade") ? g("--image1", `url(${et[c]})`, r) : g("--image2", `url(${et[c]})`, r),
                            (h = c);
                    },
                    b = () => {
                        (c = (c + 1) % et.length), f(r, "crossfade");
                    },
                    l = () => {
                        wt(et),
                            g("--image1", `url(${et[0]})`, r),
                            g("--image2", `url(${et[1]})`, r),
                            r.addEventListener(oe, v),
                            setInterval(b, 2500);
                    },
                    d = await se(s.backgroundImage),
                    u = await se(s.backgroundImagePartner);
                if (d) {
                    if ((n(r, "image-set"), g("--bgImage", `url(${d})`, r), s.partnerCommander)) {
                        const n = e(["partner-image"]);
                        n.addEventListener(o, (e) => {
                            e.stopPropagation(), ps(s, a, !0);
                        }),
                            t([n], r),
                            u ? g("--image1", `url(${u})`, r) : l();
                    }
                } else l();
                r.addEventListener(o, () => ps(s, a)), t([m], r), t([r, p], i);
            },
            Pt = (e, t, n, o, i = !1) => {
                (n.counters = n.counters ?? {}),
                    o.forEach((o) => {
                        const r = o.includes("tax") ? e * 2 : e;
                        let a = Math.max((n.counters[o] ?? 0) + r, 0);
                        o === "speed" && (a = Math.min(a, 4)),
                            (n.counters[o] = a),
                            i ? s(t, n.counters[o]) : (S(t, n.counters[o]), ue(n)),
                            o === "poison" && n.counters[o] === 10 && e > 0 && ft(n, "poison");
                    }),
                    U(),
                    or();
            },
            xs = (s, a) => {
                const c = a[0],
                    l = e(["counter-entry", c]),
                    u = e(["increase-counter"]),
                    h = e(["decrease-counter"]),
                    m = e(["counter-display-wrapper"]),
                    f = e(["increase-label"]),
                    p = e(["decrease-label"]),
                    g = e(["counter-display"]),
                    r = {},
                    d = {};
                return (
                    ((c === "tax-2" && s.partnerCommander) || c === "tax-1" || c === "poison") && n(l, "visible"),
                    a.forEach((a) => {
                        (r[a] = e(["counter-number"])),
                            (d[a] = e(["counter-icon", a.startsWith("tax-") ? "tax" : a])),
                            s.partnerCommander && n(d[a], a);
                        let c = s?.counters?.[a] ?? 0;
                        S(r[a], c),
                            S(p, "-"),
                            S(f, "+"),
                            h.addEventListener(o, () => {
                                Pt(-1, r[a], s, [a]), i("Decrease Counter");
                            }),
                            u.addEventListener(o, () => {
                                Pt(1, r[a], s, [a]), i("Increase Counter");
                            }),
                            t([r[a], d[a]], g);
                    }),
                    t([f, g, p], m),
                    t([u, m, h], l),
                    l
                );
            },
            Cs = ["poison", "tax-1", "tax-2"],
            jt = [
                "storm",
                "xp",
                "energy",
                "rad",
                "treasure",
                "clue",
                "food",
                "aC",
                "bC",
                "cC",
                "W",
                "U",
                "B",
                "R",
                "G",
                "C",
                "speed",
                "gold",
                "map",
                "blood",
            ],
            Ra = [
                "tax-1",
                "poison",
                "storm",
                "xp",
                "energy",
                "rad",
                "treasure",
                "clue",
                "food",
                "speed",
                "gold",
                "map",
                "blood",
                "W",
                "U",
                "B",
                "R",
                "G",
                "C",
                "aC",
                "bC",
                "cC",
            ],
            La = {
                "tax-1": "Commander Tax",
                poison: "Poison",
                storm: "Storm",
                xp: "Experience",
                energy: "Energy",
                rad: "Radiation",
                treasure: "Treasure",
                clue: "Clue",
                food: "Food",
                aC: "Counter A",
                bC: "Counter B",
                cC: "Counter C",
                W: "White Mana",
                U: "Blue Mana",
                B: "Black Mana",
                R: "Red Mana",
                G: "Green Mana",
                C: "Colorless Mana",
                speed: "Speed",
                gold: "Gold",
                map: "Map",
                blood: "Blood",
            },
            za = (e) => (e === "tax-1" ? "tax" : e),
            Ms = (e) => {
                const t = [...a.persistentCounters];
                return e.partnerCommander && t.includes("tax-1") && !t.includes("tax-2") && t.push("tax-2"), t;
            },
            Fs = (e) => {
                (e.counters = e.counters ?? {}),
                    Ms(e).forEach((t) => {
                        e.counters[t] === void 0 && (e.counters[t] = 0);
                    });
            },
            Ts = () => {
                j.forEach((e) => {
                    Fs(e), e.playerCard && ue(e);
                }),
                    U();
            },
            Fa = () => {
                const l = e(["edit-defeat-messages-overlay", "persistent-counters-overlay"]),
                    u = e(["close-edit-defeat-messages-overlay"]),
                    h = e(["wrapper"]),
                    f = e(["persistent-counters-title"]),
                    p = e(["persistent-counters-subtitle"]),
                    g = e(["persistent-counters-list"]);
                s(f, "Pinned Counters"),
                    s(p, "Pick counters to keep on every card, even between games."),
                    Ra.forEach((l) => {
                        const u = e(["persistent-counter-row"]),
                            d = e(["toggle"]),
                            f = e(["counter-icon", za(l)]),
                            h = e(["persistent-counter-name"]);
                        s(h, La[l] || l),
                            a.persistentCounters.includes(l) && n(d),
                            u.addEventListener(o, () => {
                                const t = a.persistentCounters.indexOf(l);
                                t === -1
                                    ? (a.persistentCounters.push(l), n(d))
                                    : (a.persistentCounters.splice(t, 1),
                                      r(d),
                                      j.forEach((e) => {
                                          e.counters && e.counters[l] === 0 && delete e.counters[l],
                                              l === "tax-1" &&
                                                  e.counters &&
                                                  e.counters["tax-2"] === 0 &&
                                                  delete e.counters["tax-2"];
                                      })),
                                    c(L, a),
                                    Ts(),
                                    ei();
                                const e = m.querySelector(".persistent-counters-setting");
                                e && (a.persistentCounters.length > 0 ? n(e) : r(e)),
                                    i("Toggle Persistent Counter: " + l);
                            }),
                            t([d, f, h], u),
                            t([u], g);
                    }),
                    u.addEventListener(o, () => P(l)),
                    t([f, p, g], h),
                    t([h, u], l),
                    t([l], d),
                    i("Persistent Counters Overlay", F);
            },
            Ds = (e, t, s, o) => {
                o ? (e.counters || (e.counters = {}), (e.counters[t] = 0)) : e.counters && delete e.counters[t];
                const a = s.querySelector(".counter-picker");
                if (a) {
                    const e = Array.from(a.querySelectorAll(".counter-icon")).find((e) => e.classList.contains(t));
                    e && (o ? n(e) : r(e));
                }
                const i = s.querySelector(`.counter-entry.${t}`);
                i && (o ? (n(i, "visible"), S(i.querySelector(".counter-number"), 0)) : r(i, "visible")), U();
            },
            Ma = (s, i) => {
                const a = e(["counter-picker"]),
                    r = Math.ceil(jt.length / 2),
                    c = (a) => {
                        const r = e(["counter-row"]);
                        return (
                            a.forEach((a) => {
                                const c = e(["counter-icon", a]);
                                t([c], r),
                                    i.counters &&
                                        i.counters[a] !== void 0 &&
                                        (n(c), n(s.querySelector(`.counter-entry.${a}`), "visible")),
                                    c.addEventListener(o, () => {
                                        const e = i.counters && i.counters[a] !== void 0;
                                        Ds(i, a, s, !e);
                                    });
                            }),
                            r
                        );
                    },
                    l = c(jt.slice(0, r)),
                    d = c(jt.slice(r));
                t([l, d], a), t([a], s);
            },
            ct = (t) => {
                const n = e(["option-entry", t]),
                    s = e(["option-entry-icon"]),
                    o = e(["option-entry-text"]);
                return { entry: n, icon: s, text: o };
            },
            is = (e) => t([e.icon, e.text], e.entry),
            Ps = () => {
                const e = ct("background"),
                    t = ct("kill"),
                    n = ct("partner"),
                    s = ct("name"),
                    o = [e, t, n, s];
                return (
                    o.forEach((e) => is(e)),
                    {
                        backgroundEntry: e.entry,
                        backgroundEntryIcon: e.icon,
                        backgroundEntryText: e.text,
                        killEntry: t.entry,
                        killEntryIcon: t.icon,
                        killEntryText: t.text,
                        partnerToggleEntry: n.entry,
                        partnerToggleEntryIcon: n.icon,
                        partnerToggleEntryText: n.text,
                        nameEntry: s.entry,
                        nameEntryIcon: s.icon,
                        nameEntryText: s.text,
                    }
                );
            },
            os = (e) => {
                const t = e.playerCard && e.playerCard.children[0];
                if (!t) return;
                const o = t.querySelector(".player-card-name");
                if (!o) return;
                const i = (e.nickname || "").trim();
                s(o, i), ut(t, "has-name", !!i && a.showPlayerNamesOnCard !== !1);
                const n = t.querySelector(".player-life-count");
                n &&
                    ((n.style.opacity = "0.999"),
                    requestAnimationFrame(() => {
                        n.style.opacity = "";
                    }));
            },
            ss = () => b(vn) || [],
            Bs = (e) => {
                if (((e = (e || "").trim()), !e)) return;
                const t = [e, ...ss().filter((t) => t.toLowerCase() !== e.toLowerCase())].slice(0, 50);
                c(vn, t);
                const n = ns().filter((t) => t.toLowerCase() !== e.toLowerCase());
                c(Nn, n);
            },
            ns = () => b(Nn) || [],
            Aa = (e) => {
                if (((e = (e || "").trim()), !e)) return;
                const n = ss().filter((t) => t.toLowerCase() !== e.toLowerCase());
                c(vn, n);
                const t = ns();
                t.some((t) => t.toLowerCase() === e.toLowerCase()) || (t.push(e), c(Nn, t)), i("Delete Player Name");
            },
            Ws = (e = "") => {
                const n = [],
                    t = new Set([e.trim().toLowerCase()]);
                j.forEach((e) => {
                    const n = (e.nickname || "").trim().toLowerCase();
                    n && t.add(n);
                }),
                    ns().forEach((e) => t.add(e.trim().toLowerCase()));
                const s = (e) => {
                    e = (e || "").trim();
                    const s = e.toLowerCase();
                    if (!e || t.has(s)) return;
                    t.add(s), n.push(e);
                };
                return (
                    ss().forEach(s),
                    (R || [])
                        .slice()
                        .reverse()
                        .forEach((e) => (e.players || []).forEach((e) => s(e.nickname))),
                    n.slice(0, 100)
                );
            },
            Us = async (c, l) => {
                const h = Rt(c, "option-card"),
                    d = e(["option-card-inner"]),
                    {
                        backgroundEntry: A,
                        backgroundEntryIcon: f,
                        backgroundEntryText: k,
                        killEntry: p,
                        killEntryIcon: v,
                        killEntryText: F,
                        partnerToggleEntry: y,
                        partnerToggleEntryIcon: _,
                        partnerToggleEntryText: M,
                        nameEntry: O,
                        nameEntryText: x,
                    } = Ps(),
                    E = e(["divider"]),
                    u = {},
                    m = [],
                    S = (e) => {
                        let t = 0;
                        if (
                            (e.querySelectorAll(".counter-number .font").forEach((e) => {
                                e.classList.forEach((e) => {
                                    if (e.startsWith("char-")) {
                                        const n = parseInt(e.replace("char-", ""), 10);
                                        isNaN(n) || (t += n);
                                    }
                                });
                            }),
                            a.keepZeroCountersOnPlayerCard && t === 0)
                        ) {
                            const n = Array.from(e.classList),
                                t = n.find((e) => Cs.includes(e) || jt.includes(e));
                            if (t && c.counters && c.counters[t] !== void 0) return !0;
                        }
                        return t !== 0;
                    },
                    w = await se(c.backgroundImage),
                    b = await se(c.backgroundImagePartner),
                    T = (t, a, r) => {
                        if (!go(t) && a.length === 0) return;
                        const d = va(t),
                            l = [],
                            u = ct("load-profile"),
                            c = ct("save-profile"),
                            m = () => {
                                is(u),
                                    s(u.text, "Load Profile"),
                                    u.entry.addEventListener(o, () => Ks(t)),
                                    l.push(u.entry);
                            },
                            f = () => {
                                is(c),
                                    s(c.text, "Save Profile"),
                                    c.entry.addEventListener(o, () => {
                                        Gt(
                                            "Name this profile (optional)",
                                            "Profile name",
                                            (e) => {
                                                (d.name = (e || "").trim()),
                                                    a.push(d),
                                                    Ye(),
                                                    n(h, Ee),
                                                    n(c.entry, "saved"),
                                                    s(c.text, "Saved"),
                                                    i("Save Profile"),
                                                    setTimeout(() => {
                                                        t.playerCardInnerClick();
                                                    }, 1e3);
                                            },
                                            ho(d),
                                            !0
                                        );
                                    }),
                                    l.push(c.entry);
                            };
                        a.length !== 0 && m(),
                            go(t) && !Jn(d, a) && f(),
                            l.length !== 0 && (l.push(e(["divider", "equal"])), Ao(l, r));
                    };
                Cs.forEach((e) => {
                    u[e] = xs(c, [e]);
                }),
                    jt.forEach((e) => {
                        u[e] = xs(c, [e]);
                    }),
                    s(k, "Background"),
                    w &&
                        (n(f, "background-image"),
                        g("--bgImage", `url(${w})`, f),
                        c.partnerCommander && (n(f, "partner"), b && g("--bgImagePartner", `url(${b})`, f))),
                    s(F, c.alive ? "Kill" : "Unkill"),
                    c.alive ? r(v) : n(v),
                    s(M, "Partners"),
                    c.partnerCommander ? n(_) : r(_),
                    s(x, "Name"),
                    O.addEventListener(o, () => {
                        Gt(
                            "Player name",
                            "Enter a name",
                            (e) => {
                                (c.nickname = e.trim()), Bs(c.nickname), os(c), U(), i("Set Player Name");
                            },
                            c.nickname || "",
                            !0,
                            Ws(c.nickname || ""),
                            !0
                        ),
                            c.playerCardInnerClick();
                    }),
                    f.addEventListener(o, async () => {
                        const a = Rt(c, "color-card"),
                            s = e(["color-card-inner"]);
                        await Pa(c, s, l),
                            Va(c, s, l),
                            n(h, Z),
                            we.forEach((n) => {
                                const a = e(["color-entry"]);
                                C(a, "backgroundColor", `var(--${n})`),
                                    a.addEventListener(o, () => {
                                        ds(n, c, l),
                                            j.forEach((e) => me(e)),
                                            c.playerCardInnerClick(),
                                            i("Set Background");
                                    }),
                                    t([a], s);
                            }),
                            t([s], a),
                            t([a], c.playerCard),
                            i("Open Background Options");
                    }),
                    p.addEventListener(o, () => {
                        c.alive
                            ? ((c.alive = !1), Hs(), t([Jt(be, c)], l), n(l, be))
                            : ((c.alive = !0), l.querySelector(".killed-overlay").remove(), r(l, be), it(c)),
                            U(),
                            Zt(),
                            c.playerCardInnerClick(),
                            i("Kill Player");
                    }),
                    y.addEventListener(o, () => {
                        (c.partnerCommander = !c.partnerCommander),
                            c.partnerCommander ||
                                ((c.backgroundImagePartner = !1),
                                (c.counters = c.counters || {}),
                                (c.counters["tax-2"] = 0),
                                Tt(c)),
                            Fs(c),
                            U(),
                            ue(c),
                            me(c),
                            c.playerCardInnerClick(),
                            i("Toggle Partner Commander");
                    }),
                    r(h, Ee);
                const z = a.showPlayerNamesOnCard !== !1;
                return (
                    t([A, p, y, ...(z ? [O] : []), E], d),
                    T(c, K, d),
                    Object.keys(u).forEach((e) => {
                        const n = S(u[e]);
                        n ? m.push(u[e]) : t([u[e]], d), t([u[e]], d);
                    }),
                    m.length > 0 &&
                        (m.unshift(e(["divider", "leading-divider"])),
                        m.push(e(["divider"])),
                        m.reverse().forEach((e) => {
                            Ao([e], d);
                        })),
                    Ma(d, c),
                    t([d], h),
                    t([h], c.playerCard),
                    gi(d),
                    fn(et, !1),
                    ht(Z),
                    h
                );
            },
            Ks = (a = !1) => {
                const l = e(["load-profile-overlay"]),
                    O = e(["close-load-profile-overlay"]),
                    _ = e(["profile-overlay-header"]),
                    p = e(["profile-overlay-title"]),
                    y = e(["profile-search-wrapper"]),
                    u = e(["profile-search"], M),
                    m = e(["profile-list"]),
                    b = e(["profile-overlay-footer"]),
                    v = e(["import-profile"]),
                    h = e(["export-all-profiles"]);
                let f = "";
                s(p, "Profiles"),
                    n(p, "has-help"),
                    p.addEventListener(o, () => ka()),
                    (u.type = "text"),
                    (u.placeholder = "Search profiles");
                const x = () => [...K.filter((e) => e.favorite), ...K.filter((e) => !e.favorite)],
                    C = (e) => !f || (e.name || ho(e)).toLowerCase().includes(f),
                    E = (e) => {
                        (a.background = e.background),
                            ds(a.background, a, a.playerCard.children[0]),
                            (a.backgroundImage = e.backgroundImage),
                            (a.backgroundImagePartner = e.backgroundImagePartner),
                            (a.partnerCommander = e.partnerCommander),
                            Tt(a),
                            U(),
                            j.forEach((e) => me(e)),
                            a.playerCardInnerClick(),
                            P(l),
                            i("Load Profile");
                    },
                    k = (n) => {
                        const a = e(["profile-action-sheet"]),
                            l = e(["profile-action-panel"]),
                            u = e(["profile-action-title"]);
                        s(u, n.name || "Profile");
                        const r = (t, n, i) => {
                                const a = e(["profile-action", n]);
                                return s(a, t), a.addEventListener(o, i), a;
                            },
                            h = r(n.name ? "Rename" : "Name", "rename", () => {
                                P(a),
                                    Gt(
                                        n.name ? "Rename profile" : "Name this profile",
                                        "Profile name",
                                        (e) => {
                                            (n.name = (e || "").trim()), Ye(), c();
                                        },
                                        n.name,
                                        !0
                                    );
                            }),
                            m = r("Duplicate", "duplicate", () => {
                                const e = Xe({ ...At(n), id: void 0 });
                                (e.name = n.name ? `${n.name} copy` : ""),
                                    K.splice(K.indexOf(n) + 1, 0, e),
                                    Ye(),
                                    P(a),
                                    c(),
                                    i("Duplicate Profile");
                            }),
                            f = r(n.favorite ? "Unfavorite" : "Favorite", "favorite", () => {
                                (n.favorite = !n.favorite), Ye(), P(a), c(), i("Favorite Profile");
                            }),
                            p = r("Share", "share", () => {
                                io(so({ ...At(n), name: n.name })), P(a), i("Share Profile");
                            }),
                            g = r("Delete", "delete", () => {
                                const e = K.indexOf(n);
                                e !== -1 && K.splice(e, 1), Ye(), P(a), c(), i("Delete Profile");
                            });
                        a.addEventListener(o, (e) => {
                            e.target === a && P(a);
                        }),
                            t([u, h, m, f, p, g], l),
                            t([l], a),
                            t([a], d);
                    },
                    A = (i) => {
                        const r = e(["profile-entry"]),
                            c = e(["starting-life-display"]),
                            l = e(["profile-menu-btn"]),
                            u = e(["partner-icon"]);
                        i.partnerCommander && n(r, "partner-active"),
                            i.favorite && n(r, "favorited"),
                            S(c, te),
                            vs(i.background, r),
                            js(i.backgroundImage, i.backgroundImagePartner, r),
                            a && r.addEventListener(o, () => E(i)),
                            l.addEventListener(o, (e) => {
                                e.stopPropagation(), k(i);
                            });
                        const d = [c, u, l];
                        if (i.favorite || i.name) {
                            const o = e(["profile-top-left"]),
                                n = [];
                            if (i.favorite) {
                                const t = e(["profile-favorite-star"]);
                                s(t, "★"), n.push(t);
                            }
                            if (i.name) {
                                const t = e(["profile-name"]);
                                s(t, i.name), n.push(t);
                            }
                            t(n, o), d.push(o);
                        }
                        return t(d, r), r;
                    },
                    w = () =>
                        requestAnimationFrame(() => {
                            g("--profile-header-h", `${_.offsetHeight}px`, l),
                                g("--profile-footer-h", `${b.offsetHeight}px`, l);
                        }),
                    c = () => {
                        ot(m);
                        const s = K.some((e) => e.name);
                        if ((s || ((f = ""), (u.value = "")), ut(y, "hidden", !s), K.length === 0)) {
                            const s = e(["empty-profile-text"]);
                            (s.innerHTML =
                                "No profiles yet <span>Customize a player card, then tap “Save Profile” to create one.</span>"),
                                t([s], m),
                                n(h, "hidden"),
                                w();
                            return;
                        }
                        r(h, "hidden");
                        const o = x().filter(C);
                        if (o.length === 0) {
                            const n = e(["empty-profile-text"]);
                            (n.innerHTML = "No matches"), t([n], m), w();
                            return;
                        }
                        o.forEach((e) => t([A(e)], m)), w();
                    };
                u.addEventListener(M, () => {
                    (f = u.value.trim().toLowerCase()), c();
                }),
                    (Xs = (e) => {
                        const t = Ys(e);
                        if (Jn(t, K)) {
                            _e("Profile already exists", "error");
                            return;
                        }
                        K.push(Xe({ ...t, id: void 0 })), Ye(), c(), _e("Profile imported"), i("Import Profile");
                    }),
                    (Qs = (e) => {
                        let t = 0,
                            n = 0;
                        e.forEach((e) => {
                            if (!Gs(e)) {
                                n++;
                                return;
                            }
                            const s = Ys(e);
                            if (Jn(s, K)) {
                                n++;
                                return;
                            }
                            K.push(Xe({ ...s, id: void 0 })), t++;
                        }),
                            t > 0 && (Ye(), c()),
                            t > 0 && n > 0
                                ? _e(`Imported ${t} ${t === 1 ? "profile" : "profiles"} (${n} skipped)`)
                                : t > 0
                                  ? _e(`Imported ${t} ${t === 1 ? "profile" : "profiles"}`)
                                  : _e("No new profiles to import", "error"),
                            i("Import Multiple Profiles");
                    }),
                    s(v, "Import"),
                    v.addEventListener(o, () => {
                        Gt("Paste profile code", "Profile code", (e) => {
                            if (!e) return;
                            const t = Oa(e);
                            if (!t) return;
                            t.version && Array.isArray(t.profiles)
                                ? Qs(t.profiles)
                                : Gs(t)
                                  ? Xs(t)
                                  : _e("Invalid profile code", "error");
                        });
                    }),
                    s(h, "Export All"),
                    h.addEventListener(o, () => {
                        const e = { version: 1, profiles: K };
                        io(so(e), `Copied ${K.length} ${K.length === 1 ? "profile" : "profiles"} to clipboard`),
                            i("Export All Profiles");
                    }),
                    O.addEventListener(o, () => P(l)),
                    t([u], y),
                    t([p, y], _),
                    t([v, h], b),
                    t([_, m, b, O], l),
                    t([l], d),
                    c(),
                    i("Load Profile", F);
            },
            qs = (e) => e.startsWith("indexeddb"),
            Ys = (e) => {
                let t = !1;
                return (
                    e.backgroundImage && qs(e.backgroundImage) && ((e.backgroundImage = !1), (t = !0)),
                    e.backgroundImagePartner &&
                        qs(e.backgroundImagePartner) &&
                        ((e.backgroundImagePartner = !1), (t = !0)),
                    t && _e("Some images could not be imported", "error"),
                    e
                );
            },
            Gs = (e) =>
                typeof e.background == "string" &&
                (typeof e.backgroundImage == "string" || e.backgroundImage === !1) &&
                (typeof e.backgroundImagePartner == "string" || e.backgroundImagePartner === !1) &&
                typeof e.partnerCommander == "boolean";
        let Xs, Qs;
        const ka = () => {
                const n = e(["help-overlay"]),
                    r = e(["help-content"]),
                    a = e(["help-cta"]);
                (r.innerHTML = `
      <h2>Profiles</h2>
      <p>A profile is a saved card look — a color or image you can reuse for any player.</p>
      <h3>Create</h3>
      <p>Swipe a player card to open its options, set a color or image, then tap "Save Profile" and name it (optional).</p>
      <h3>Apply</h3>
      <p>Swipe a player card, tap "Load Profile", and pick one to apply it to that player.</p>
      <h3>Manage</h3>
      <p>Tap the ⋯ on any profile to rename, duplicate, favorite or delete it. Favorites stay pinned to the top.</p>
      <h3>Share</h3>
      <p>Open a profile's ⋯ menu and choose Share to copy its code, then send it to a friend. "Export All" copies your whole collection, and "Import" adds profiles from a pasted code.</p>
    `),
                    s(a, "Okay, got it"),
                    a.addEventListener(o, () => P(n)),
                    t([r, a], n),
                    t([n], d),
                    i("Import Export Help", F);
            },
            Js = ({ initialValue: i, confirmText: a, onConfirm: c }) => {
                const u = e(["change-life-totals-overlay"]),
                    p = e(["overlay-wrapper"]),
                    g = e(["input-wrapper"]),
                    m = e(["life-count-display"]),
                    b = [1, 2, 3, 4, 5, 6, 7, 8, 9, "empty", 0, "del"],
                    v = e(["button-wrapper"]),
                    f = e(["cancel"]),
                    h = e(["confirm"]);
                let l = i;
                return (
                    b.forEach((s) => {
                        const o = e([N]),
                            a = new Hammer(o);
                        if (typeof s == "number") S(o, s);
                        else {
                            if (s === "del") {
                                const n = e(["icon"]);
                                t([e(["x-icon"])], n), t([n], o);
                            }
                            n(o, s);
                        }
                        if (s !== "empty") {
                            let e;
                            a.on("tap", () => {
                                if (s === "del") {
                                    const e = l.toString();
                                    if (e.length <= 1) l = 0;
                                    else if (e.startsWith("-") && e.length === 2) l = 0;
                                    else if (e.startsWith("-")) {
                                        const t = e.slice(0, -1);
                                        l = t === "-" ? 0 : parseInt(t, 10);
                                    } else l = parseInt(e.slice(0, -1) || "0", 10);
                                } else {
                                    const e = l.toString();
                                    l = e === "0" ? s : parseInt(e + s, 10);
                                }
                                (l = parseInt(l)),
                                    l === i ? r(h) : n(h),
                                    S(m, l),
                                    n(o),
                                    e && clearTimeout(e),
                                    (e = setTimeout(() => r(o), 150));
                            });
                        }
                        t([o], g);
                    }),
                    S(m, l),
                    s(f, "Cancel"),
                    s(h, a),
                    h.addEventListener(o, () => {
                        if (l === i) return;
                        c(l), P(u);
                    }),
                    f.addEventListener(o, () => P(u)),
                    t([f, h], v),
                    t([m, g, v], p),
                    t([p], u),
                    t([u], d),
                    u
                );
            },
            xa = (a, c) => {
                a = a.slice();
                let l = !1;
                const u = e(["name-list-overlay"]),
                    m = e(["wrapper"]),
                    f = e(["name-list-headline"]);
                s(f, "Player names");
                const p = e(["name-list"]),
                    h = e(["edit-names-btn"]);
                s(h, "Edit");
                const v = () => {
                    if (a.length) return;
                    (l = !1), r(u, "edit"), n(h, "hidden"), s(h, "Edit");
                };
                a.forEach((n) => {
                    const i = e(["input-chip"]),
                        r = e(["name-chip-label"]);
                    s(r, n);
                    const d = e(["name-chip-delete"]);
                    t([r, d], i),
                        i.addEventListener(o, () => {
                            if (l) {
                                Aa(n), (a = a.filter((e) => e !== n)), i.remove(), v();
                                return;
                            }
                            c(n), P(u);
                        }),
                        t([i], p);
                }),
                    h.addEventListener(o, () => {
                        (l = !l), ut(u, "edit", l), s(h, l ? "Done" : "Edit");
                    });
                const g = e(["close-name-list-overlay"]);
                g.addEventListener(o, () => P(u)), t([f, p], m), t([m, h, g], u), t([u], d), i("Player Names List", F);
            },
            to = (n, i) => {
                if (!n.length) return null;
                const r = e(["input-chips-section"]),
                    c = e(["input-chips-label"]);
                s(c, "Recently used names");
                const a = e(["input-chips"]);
                if (
                    (n.slice(0, 8).forEach((n) => {
                        const r = e(["input-chip"]);
                        s(r, n), r.addEventListener(o, () => i(n)), t([r], a);
                    }),
                    n.length > 8)
                ) {
                    const r = e(["input-chip", "view-all"]);
                    s(r, "View all"), r.addEventListener(o, () => xa(n, i)), t([r], a);
                }
                return t([c, a], r), r;
            },
            Gt = (a, c, l, u = "", h = !1, m = [], f = !1) => {
                const p = e(["input-overlay"]);
                f && n(p, "large-input");
                const j = e(["overlay-text"]),
                    g = e([], M),
                    y = e(["btn-wrapper"]),
                    b = e([N, "cancel"]),
                    v = e([N, "confirm", V]),
                    w = (e) => {
                        l(e), P(p), i("Input Prompt Chip");
                    },
                    _ = to(m, w);
                s(j, a),
                    s(v, "OK"),
                    s(b, "Cancel"),
                    (g.placeholder = c),
                    u && (g.value = u),
                    (u || h) && r(v, V),
                    g.addEventListener(M, () => {
                        g.value.length > 0 || h ? r(v, V) : n(v, V), i("Input Prompt", M, "Profiles");
                    }),
                    v.addEventListener(o, () => {
                        l(g.value), P(p), i("Submit Profile Input");
                    }),
                    b.addEventListener(o, () => {
                        P(p), i("Cancel Profile Input");
                    }),
                    t([b, v], y),
                    t([j, g, y, ...(_ ? [_] : [])], p),
                    t([p], d),
                    i("Prompt Profile Input", F);
            },
            so = (e) => btoa(JSON.stringify(e)),
            Oa = (e) => {
                try {
                    return JSON.parse(atob(e));
                } catch {
                    return _e("Invalid profile code", "error"), !1;
                }
            },
            io = (e, t = "Copied code to clipboard") => {
                navigator.clipboard.writeText(e).then(() => {
                    _e(t);
                });
            },
            ao = (e) => {
                n(e, "hidden"),
                    e.addEventListener(
                        oe,
                        () => {
                            e.remove();
                        },
                        w
                    );
            },
            ro = (e, t) => {
                setTimeout(() => {
                    ao(e);
                }, t);
            },
            _e = (i, a = "success") => {
                const r = e(["toast", a]);
                s(r, i), r.addEventListener(o, () => ao(r));
                const c = m.querySelectorAll(".toast");
                let l = !1;
                if (
                    (c.forEach((e) => {
                        e.textContent === i && (l = !0);
                    }),
                    l)
                )
                    return;
                c.length > 0 ? (n(c[0], "above"), t([r], d, c[0]), ro(r, 6e3)) : (t([r], d), ro(r, 3e3));
            },
            ja = () =>
                typeof crypto != "undefined" && crypto.randomUUID
                    ? crypto.randomUUID()
                    : `p-${Date.now()}-${Math.floor(Math.random() * 1e9)}`,
            At = (e) => ({
                background: e.background,
                backgroundImage: e.backgroundImage,
                backgroundImagePartner: e.backgroundImagePartner,
                partnerCommander: e.partnerCommander,
            }),
            ho = (e) =>
                e.backgroundImage
                    ? ""
                    : typeof e.background == "string" && e.background.startsWith("#")
                      ? ""
                      : typeof e.background == "string"
                        ? e.background.replace(/([a-z])([A-Z])/g, "$1 $2").replace(/^\w/, (e) => e.toUpperCase())
                        : "",
            Xe = (e) => ({
                id: e.id || ja(),
                name: typeof e.name == "string" ? e.name : "",
                favorite: !!e.favorite,
                createdAt: e.createdAt || Date.now(),
                ...At(e),
            }),
            Jn = (e, t) => t.some((t) => JSON.stringify(At(t)) === JSON.stringify(At(e))),
            va = (e) =>
                Xe({
                    background: e.background,
                    backgroundImage: e.backgroundImage,
                    backgroundImagePartner: e.backgroundImagePartner,
                    partnerCommander: e.partnerCommander,
                }),
            go = (e) => !!(e.backgroundImage || e.background.startsWith("#")),
            Rt = (e, t) => {
                const o = e.playerCard.children[0],
                    s = o.cloneNode();
                return n(s, t), (s.style = ""), s;
            },
            ga = (e, t, n) => {
                switch (t) {
                    case fe:
                    case de:
                        Us(e, n);
                        break;
                    case Y:
                    case ee:
                        Ja(e);
                        break;
                }
            },
            jo = (e) => {
                let t = e.nextSibling;
                for (; t; ) {
                    let e = t;
                    (t = t.nextSibling), e.remove();
                }
            },
            yo = (e, t, s, c, l, d, u, h = 1, f = null) => {
                let p = 0,
                    v = null;
                const g = (e) => {
                        p += e;
                        const o = e > 0,
                            s = p > 0,
                            i = p === 0 ? (o ? "+" : "-") : `${s ? "+" : ""}${p}`,
                            t = s ? l.increase : l.decrease;
                        r(l.increase),
                            r(l.decrease),
                            n(t),
                            S(t, i),
                            p === 0 && (S(l.increase, "+"), S(l.decrease, "-")),
                            clearTimeout(v),
                            (v = setTimeout(() => {
                                (p = 0), S(l.increase, "+"), S(l.decrease, "-"), r(t);
                            }, 1e3));
                    },
                    b = (s, c) => {
                        let b,
                            p = !1,
                            j = null;
                        const w = s.closest(".player-card-inner"),
                            O = w ? w.querySelectorAll(".increase-button, .decrease-button") : [],
                            l = () => {
                                b && (clearInterval(b), (b = null)), clearTimeout(j), r(s), (p = !1);
                                const t = e.playerCard.children[0];
                                t && delete t.dataset.longPressing;
                            },
                            x = (e) => {
                                e !== s && (p || _(s)) && l();
                            };
                        O.forEach((e) => {
                            e.addEventListener(o, () => x(e));
                        });
                        const y = (t) => {
                            const n = e.playerCard.children[0];
                            n && delete n.dataset.longPressing, !s.contains(t.target) && (p || _(s)) && l();
                        };
                        m.addEventListener("pointerup", y),
                            m.addEventListener(qe, y),
                            m.addEventListener("visibilitychange", () => {
                                m.hidden && l();
                            });
                        const v = new Hammer(s);
                        v.on("tap", () => {
                            if (p) {
                                l();
                                return;
                            }
                            n(s), clearTimeout(j), (j = setTimeout(() => r(s), 200));
                            const m = a.customSingleTapEnabled ? a.customSingleTapValue : 1,
                                o = c * m;
                            if (u === "commanderDamage") {
                                const a = f.commanderDamage.find((t) => t.player === e.name),
                                    n = f.playerCard.children[1];
                                if (n === void 0) return;
                                const r = n.querySelectorAll(".damage-display"),
                                    l = a?.damage[`commander${h}`] ?? 0;
                                l + o >= 0 &&
                                    (d(o, t, e, u, h),
                                    g(o, s),
                                    f !== e
                                        ? bt(n, c > 0 ? "commander-damage-received" : "commander-damage-removed")
                                        : r.forEach((e) => {
                                              bt(e, c > 0 ? "own-damage-received" : "own-damage-removed");
                                          }),
                                    i(c > 0 ? "Increase Commander Damage" : "Decrease Commander Damage"));
                            } else d(o, t, e, u, h), g(o, s), i(c > 0 ? "Increase Life" : "Decrease Life");
                        }),
                            v.on("press", () => {
                                b && l(), (p = !0), clearTimeout(j), n(s);
                                const o = e.playerCard.children[0];
                                o && (o.dataset.longPressing = "1"),
                                    (b = setInterval(() => {
                                        if (!p) {
                                            l();
                                            return;
                                        }
                                        const n = a.customLongTapEnabled ? a.customLongTapValue : 10;
                                        if (u === "commanderDamage") {
                                            const a = f.commanderDamage.find((t) => t.player === e.name),
                                                o = f.playerCard.children[1];
                                            if (o === void 0) {
                                                l();
                                                return;
                                            }
                                            const r = o.querySelectorAll(".damage-display"),
                                                m = a?.damage[`commander${h}`] ?? 0;
                                            m + c * n >= 0 &&
                                                (d(c * n, t, e, u, h),
                                                g(c * n, s),
                                                f !== e
                                                    ? bt(
                                                          o,
                                                          c > 0
                                                              ? "commander-damage-received"
                                                              : "commander-damage-removed"
                                                      )
                                                    : r.forEach((e) => {
                                                          bt(e, c > 0 ? "own-damage-received" : "own-damage-removed");
                                                      }),
                                                i(c > 0 ? "Increase Commander Damage" : "Decrease Commander Damage"));
                                        } else {
                                            const o = e.playerCard.children[0];
                                            if (
                                                (bt(o, "big-change"),
                                                d(c * n, t, e, u, h),
                                                g(c * n, s),
                                                i(c > 0 ? "Increase Life" : "Decrease Life"),
                                                _(o, "killed"))
                                            ) {
                                                l();
                                                return;
                                            }
                                        }
                                    }, 500));
                            }),
                            v.on("pressup", () => l()),
                            v.on("panend", () => l()),
                            v.on("release", () => l());
                        const C = () => {
                            m.removeEventListener("pointerup", y),
                                m.removeEventListener(qe, y),
                                O.forEach((e) => {
                                    e.removeEventListener(o, () => x(e));
                                }),
                                v.destroy(),
                                l();
                        };
                        return C;
                    };
                b(s, 1), b(c, -1);
                const j = u === "commanderDamage" ? f.commanderDamage.find((t) => t.player === e.name) : null,
                    y = u === "commanderDamage" ? (j?.damage[`commander${h}`] ?? 0) : e[u];
                S(t, y);
            },
            pa = (o, c) => {
                let y = !1,
                    R = null;
                const { colors: L, rotation: J } = G[`p${A}`][$],
                    Z = L.indexOf(!1) !== -1 && L.indexOf(!1) <= c ? -1 : 0,
                    z = J[c - Z],
                    T = () => fa(m),
                    m = e(["player-card", z]),
                    d = e(["player-card-inner"]),
                    D = e(["player-card-name"]),
                    g = e(["player-life-count"]),
                    k = e(["increase-button", "life"]),
                    E = e(["decrease-button", "life"]),
                    p = { increase: e(["life-change-feedback"]), decrease: e(["life-change-feedback"]) },
                    X = () => {
                        const e = [m.querySelector(".counters-on-card"), ...m.querySelectorAll(".counter")].filter(
                            Boolean
                        );
                        return e;
                    },
                    f = new Hammer(m),
                    B = new Hammer(g),
                    q = {
                        [u]: { [fe]: ee, [de]: Y, [Y]: fe, [ee]: de },
                        [l]: { [fe]: Y, [de]: ee, [Y]: de, [ee]: fe },
                        [Fe]: { [fe]: de, [de]: fe, [Y]: ee, [ee]: Y },
                    },
                    K = (e) => q[z]?.[e] || e;
                B.on("tap", () => {
                    if (!a.setLifeByTappingNumber) return;
                    const e = o.life;
                    Js({
                        initialValue: e,
                        confirmText: "Set Life",
                        onConfirm: (t) => {
                            const n = t - e;
                            He(),
                                (o.life = t),
                                S(g, o.life),
                                Bo(o, n),
                                U(),
                                it(o),
                                i("Set Life Manually"),
                                t === 0 && ft(o, "life");
                        },
                    }),
                        i("Set Life Manually Overlay", F);
                }),
                    m.addEventListener("pointerdown", (e) => (R = e.target)),
                    f.get("swipe").set({ enable: !1 }),
                    f.get("pan").set({ direction: Hammer.DIRECTION_ALL, threshold: 5 });
                const h = {
                    active: !1,
                    direction: null,
                    physDir: null,
                    geo: null,
                    commanderPreview: null,
                    baseTransform: Ge,
                };
                let v = 0;
                const W = (e) => {
                        const t = d.clientWidth,
                            n = d.clientHeight;
                        switch (e) {
                            case fe:
                                return { axis: "Y", target: -(n - 34) };
                            case de:
                                return { axis: "Y", target: n - 44 };
                            case Y:
                                return { axis: "X", target: -(t + 5) };
                            case ee:
                                return { axis: "X", target: t + 5 };
                        }
                    },
                    H = (e) =>
                        Math.abs(e.deltaX) > Math.abs(e.deltaY) ? (e.deltaX > 0 ? ee : Y) : e.deltaY > 0 ? de : fe,
                    I = (e, t, n) => {
                        switch (e) {
                            case ee:
                                return t;
                            case Y:
                                return -t;
                            case de:
                                return n;
                            case fe:
                                return -n;
                        }
                    },
                    V = () => {
                        const i = Rt(o, "info-card"),
                            n = e(["info-card-inner"]),
                            a = e(["info-card-text"]),
                            r = e(["return-to-game-button"]);
                        return (
                            (a.innerHTML = `<b>Commander</b>Damage you’ve received`),
                            s(r, "Return to Game"),
                            t([a, r], n),
                            t([n], i),
                            t([i], o.playerCard),
                            Jo(n),
                            Oo(n, "--font-size-commander"),
                            i
                        );
                    },
                    N = (e) => {
                        const t = K(H(e));
                        if ([Y, ee].includes(t) && (A === 1 || !o.alive)) return !1;
                        if (
                            ((h.direction = t),
                            (h.physDir = H(e)),
                            (h.geo = W(t)),
                            (h.baseTransform = vo(d)),
                            C(d, "transition", Ge),
                            n(d, `swipe-${t}`),
                            n(d, "dragging"),
                            t === fe || t === de)
                        ) {
                            const e = v;
                            Us(o, d)
                                .then((t) => {
                                    t && v !== e && t.remove();
                                })
                                .catch(() => {});
                        } else h.commanderPreview = V();
                        return !0;
                    },
                    P = (e, t) => Math.min(Math.max(I(h.physDir, e, t) / Math.abs(h.geo.target), 0), 1),
                    O = () => {
                        (h.active = !1),
                            (h.direction = null),
                            (h.physDir = null),
                            (h.geo = null),
                            (h.commanderPreview = null);
                    },
                    b = () => {
                        v++,
                            C(d, "transform", ""),
                            Array.from(d.classList).forEach((e) => {
                                e.startsWith("swipe-") && r(d, e);
                            }),
                            jo(d),
                            nn === 0 && !Po() && ht(Et);
                    };
                f.on("panstart", () => {
                    if (h.active) return;
                    if (
                        _(d) ||
                        _(d, "reset-swipe") ||
                        _(d, "commander-damage-active") ||
                        d.dataset.longPressing ||
                        X().includes(R)
                    ) {
                        h.active = !1;
                        return;
                    }
                    (h.active = !0), (h.direction = null);
                }),
                    f.on("panmove", (e) => {
                        if (!h.active) return;
                        if (!h.direction) {
                            if (Math.abs(e.deltaX) < 5 && Math.abs(e.deltaY) < 5) return;
                            if (!N(e)) {
                                h.active = !1;
                                return;
                            }
                        }
                        const n = P(e.deltaX, e.deltaY),
                            t = `translate${h.geo.axis}(${n * h.geo.target}px)`;
                        C(d, "transform", h.baseTransform === Ge ? t : `${h.baseTransform} ${t}`);
                    }),
                    f.on("panend", (e) => {
                        if (!h.active) return;
                        if (!h.direction && ((Math.abs(e.deltaX) < 5 && Math.abs(e.deltaY) < 5) || !N(e))) {
                            O();
                            return;
                        }
                        const t = P(e.deltaX, e.deltaY),
                            n = I(h.physDir, e.velocityX, e.velocityY),
                            s = t >= 0.35 || n > 0.3;
                        C(d, "transition", ""),
                            r(d, "dragging"),
                            s
                                ? h.direction === Y || h.direction === ee
                                    ? Hn()
                                        ? (h.commanderPreview && h.commanderPreview.remove(), b())
                                        : (h.commanderPreview && h.commanderPreview.remove(),
                                          x(h.direction, { buildMenu: !0, baseTransform: h.baseTransform }))
                                    : Hn()
                                      ? b()
                                      : x(h.direction, { buildMenu: !1, baseTransform: h.baseTransform })
                                : t > 0.01
                                  ? j()
                                  : b(),
                            O();
                    }),
                    f.on("pancancel", () => {
                        if (!h.active) return;
                        C(d, "transition", ""), r(d, "dragging"), h.direction && b(), O();
                    }),
                    S(p.increase, "+"),
                    S(p.decrease, "-"),
                    t([p.increase], k),
                    t([p.decrease], E),
                    yo(o, g, k, E, p, Ho, "life");
                const x = (e, { buildMenu: t = !0, baseTransform: s = null } = {}) => {
                        n(d, Ee),
                            d.addEventListener("pointerdown", j),
                            s === null ? mo(d, Pn[e]) : C(d, "transform", s === Ge ? Pn[e] : `${s} ${Pn[e]}`),
                            n(d),
                            n(d, `swipe-${e}`),
                            t && ga(o, e, d),
                            (e === fe || e === de) &&
                                (y || (++nn, (y = !0)), i("Player Option Card", "Swipe", "Player Card")),
                            d.addEventListener(
                                oe,
                                () => {
                                    r(d, Ee);
                                },
                                w
                            );
                    },
                    Q = (e) => {
                        if ([Y, ee].includes(e) && A === 1) return;
                        if ([Y, ee].includes(e) && !o.alive) return;
                        if ([Y, ee].includes(e) && Hn()) return;
                        x(e, { buildMenu: !0 });
                    },
                    j = () => {
                        v++,
                            C(d, "transform", ""),
                            r(d),
                            n(d, "reset-swipe"),
                            d.classList.forEach((e) => {
                                e.startsWith("swipe-") && r(d, e);
                            }),
                            d.removeEventListener("pointerdown", j),
                            d.addEventListener(
                                oe,
                                () => {
                                    r(d, "reset-swipe"), jo(d);
                                },
                                w
                            ),
                            y && (--nn, (y = !1)),
                            nn === 0 && !Po() && ht(Et);
                    };
                Tt(o, m, d), requestAnimationFrame(T), An(T), t([D, g, E, k], d), t([d], m);
                const M = (o.nickname || "").trim();
                return (
                    s(D, M),
                    ut(d, "has-name", !!M && a.showPlayerNamesOnCard !== !1),
                    o.alive || (n(d, be), t([Jt(be, o)], d)),
                    (o.playerCardInnerClick = () => {
                        _(d) && j();
                    }),
                    (o.triggerSwipe = (e) => {
                        Q(e);
                    }),
                    m
                );
            },
            wo = () => [k.isActive, y.isActive, O.isActive].filter(Boolean).length,
            Gn = () => wo() < 2,
            qn = () => {
                const i = e(["max-game-modes-warning"]),
                    r = e(["text"]),
                    a = e(["close"]);
                s(r, "Maximum 2 game modes can be active at once. Please disable one first."),
                    s(a, "Got it"),
                    a.addEventListener(o, () => {
                        n(i, E), i.addEventListener(x, () => i.remove(), w);
                    }),
                    t([r, a], i),
                    t([i], d);
            },
            le = () => {
                const l = m.querySelector(`.${De}`),
                    t = `${De}-`,
                    e = l || m.querySelector(`.${en}`),
                    i = G[`p${A}`][$],
                    a = "layout-other-btns-",
                    c = wo();
                let s = c >= 2 ? "multi" : "single";
                e &&
                    (e.classList.forEach((n) => {
                        n.startsWith(t) && r(e, n);
                    }),
                    s === "single"
                        ? ((s = "multi"),
                          k.isActive
                              ? n(e, t + "planechase-active")
                              : y.isActive
                                ? n(e, t + "archenemy-active")
                                : O.isActive
                                  ? n(e, t + "bounty-active")
                                  : (s = "single"))
                        : n(e, t + "both-active")),
                    d.classList.forEach((e) => {
                        e.startsWith(a) && r(d, e);
                    }),
                    ["portrait", "landscape-portrait-portrait"].includes(i.key) &&
                        e &&
                        c >= 2 &&
                        n(e, t + "move-bottom");
                const o = m.querySelector(".bounty-btn");
                o &&
                    (r(o, "bounty-btn-take-planechase-position"),
                    O.isActive && y.isActive && !k.isActive && n(o, "bounty-btn-take-planechase-position")),
                    n(d, a + i.btns[s]);
            },
            Kn = () => {
                let e = "50%";
                if (A > 2) {
                    {
                        e = `calc(${j[A - 1].playerCard.offsetHeight}px + var(--gap) / 2)`;
                        const t = G[`p${A}`][$].anchors;
                        if (t) {
                            const [n, s] = t;
                            e = `calc(${j[n].playerCard.offsetHeight + j[s].playerCard.offsetHeight}px + var(--gap) * 1.5)`;
                        }
                    }
                } else (A === 1 || $ === "portrait-portrait") && (e = "calc(10% + 100px)");
                g("--menuBtn-bottom", e, Oe);
            },
            ha = () => {
                const s = e(["players-overlay"]),
                    r = e(["players-overlay-list"]),
                    a = e(["close-players-overlay-btn"]),
                    l = [s, a];
                Object.keys(G).forEach((c) => {
                    Object.keys(G[c]).forEach((l) => {
                        if (l === "landscape-landscape-uni" || l === "landscape-landscape-sides") return;
                        if (l === "landscape") return;
                        const d = e(["layout-item", G[c][l].key]);
                        $ === l && A === Number(c.slice(1)) && n(d),
                            ($ === "landscape-landscape-uni" || $ === "landscape-landscape-sides") &&
                                l === "landscape-landscape" &&
                                A === Number(c.slice(1)) &&
                                n(d),
                            $ === "landscape" && l === "portrait" && A === Number(c.slice(1)) && n(d),
                            G[c][l].rotation.forEach((s, o) => {
                                const i = e(["card-placeholder", s]),
                                    a = G[c][l].colors[o];
                                if (
                                    (a === !1 && n(i, "empty"),
                                    l === "portrait-portrait-portrait-portrait" && Number(c.slice(1)) === 4)
                                ) {
                                    const n = e(["life-count-placeholder"]);
                                    S(n, te), t([n], i);
                                }
                                t([i], d);
                            }),
                            d.addEventListener(o, () => {
                                const e = Number(c.slice(1));
                                if (l === "portrait" && e === 1) {
                                    n(s, E),
                                        n(a, "blop-out"),
                                        setTimeout(() => {
                                            s.remove(), a.remove(), da();
                                        }, 300),
                                        i("Open 1-Player Layout Overlay");
                                    return;
                                }
                                if (l === "landscape-landscape" && e === 2) {
                                    n(s, E),
                                        n(a, "blop-out"),
                                        setTimeout(() => {
                                            s.remove(), a.remove(), ci();
                                        }, 300),
                                        i("Open 2-Player Direction Overlay");
                                    return;
                                }
                                ($ = l), (A = c.slice(1)), i("Change Layout to " + $ + " for " + A + " players"), zt();
                            }),
                            t([d], r);
                    });
                }),
                    a.addEventListener(o, () => {
                        n(s, E),
                            n(a, "blop-out"),
                            l.forEach((e) => {
                                e.addEventListener(
                                    x,
                                    () => {
                                        e.remove();
                                    },
                                    w
                                );
                            }),
                            i("Close Players Overlay");
                    }),
                    t([r, a], s),
                    t([s], d);
                const c = s.querySelector(".active");
                c && c.scrollIntoView({ behavior: "smooth", block: "center" }), i("Players Overlay", F);
            },
            da = () => {
                const a = e(["1-player-layout-overlay"]),
                    c = e(["direction-overlay-headline"]),
                    l = e(["direction-overlay-explanation"]),
                    u = e(["direction-overlay-info"]),
                    h = e(["direction-overlay-list"]),
                    r = e(["close-players-overlay-btn"]),
                    f = [a, r];
                s(c, "Player Card Layout"),
                    s(l, "Select the layout for your player card."),
                    (u.innerHTML =
                        "<b>i</b> The landscape layout uses vertical tap areas instead of side-by-side tap areas.");
                const p = [
                    { key: "portrait", label: "Portrait", layoutType: "portrait" },
                    { key: "landscape", label: "Landscape", layoutType: "landscape" },
                ];
                p.forEach((a) => {
                    const c = e(["direction-option-wrapper"]),
                        r = e(["direction-item", "layout-item", a.key]);
                    $ === a.layoutType && n(r),
                        G.p1[a.layoutType].rotation.forEach((n, s) => {
                            const o = e(["card-placeholder", n]),
                                c = G.p1[a.layoutType].colors[s],
                                i = e(["life-count-placeholder"]);
                            S(i, Ct(te)), t([i], o), t([o], r);
                        });
                    const l = e(["direction-label"]);
                    s(l, a.label),
                        r.addEventListener(o, () => {
                            ($ = a.layoutType),
                                (A = 1),
                                i("Change Layout to " + a.layoutType + " (1-player " + a.label + ")"),
                                zt();
                        }),
                        t([r, l], c),
                        t([c], h);
                }),
                    r.addEventListener(o, () => {
                        n(a, E),
                            n(r, "blop-out"),
                            f.forEach((e) => {
                                e.addEventListener(
                                    x,
                                    () => {
                                        e.remove();
                                    },
                                    w
                                );
                            }),
                            i("Close 1-Player Layout Overlay");
                    }),
                    t([c, l, u, h, r], a),
                    t([a], d);
                const m = a.querySelector(".active");
                m && m.scrollIntoView({ behavior: "smooth", block: "center" }), i("1-Player Layout Overlay", F);
            },
            ci = () => {
                const a = e(["2-player-direction-overlay"]),
                    c = e(["direction-overlay-headline"]),
                    l = e(["direction-overlay-explanation"]),
                    u = e(["direction-overlay-list"]),
                    r = e(["close-players-overlay-btn"]),
                    m = [a, r];
                s(c, "Player Card Direction"), s(l, "Select how you want the player cards to be oriented.");
                const f = [
                    { key: "landscape-landscape", label: "Default", layoutType: "landscape-landscape" },
                    { key: "landscape-landscape-uni", label: "Same Direction", layoutType: "landscape-landscape-uni" },
                    {
                        key: "landscape-landscape-sides",
                        label: "Facing Sides",
                        layoutType: "landscape-landscape-sides",
                    },
                ];
                f.forEach((a) => {
                    const c = e(["direction-option-wrapper"]),
                        r = e(["direction-item", "layout-item", a.key]);
                    $ === a.layoutType && n(r),
                        G.p2[a.layoutType].rotation.forEach((n, s) => {
                            const o = e(["card-placeholder", n]),
                                c = G.p2[a.layoutType].colors[s],
                                i = e(["life-count-placeholder"]);
                            S(i, Ct(Se)), t([i], o), t([o], r);
                        });
                    const l = e(["direction-label"]);
                    s(l, a.label),
                        r.addEventListener(o, () => {
                            ($ = a.layoutType),
                                (A = 2),
                                i("Change Layout to " + a.layoutType + " (2-player " + a.label + ")"),
                                zt();
                        }),
                        t([r, l], c),
                        t([c], u);
                }),
                    r.addEventListener(o, () => {
                        n(a, E),
                            n(r, "blop-out"),
                            m.forEach((e) => {
                                e.addEventListener(
                                    x,
                                    () => {
                                        e.remove();
                                    },
                                    w
                                );
                            }),
                            i("Close 2-Player Direction Overlay");
                    }),
                    t([c, l, u, r], a),
                    t([a], d);
                const h = a.querySelector(".active");
                h && h.scrollIntoView({ behavior: "smooth", block: "center" }), i("2-Player Direction Overlay", F);
            },
            T = { isPaused: !1, pausedTime: 0 },
            ne = { timer: null, node: null },
            To = () => ({ startTime: Date.now(), isPaused: !1, pausedTime: 0 }),
            Ae = () => {
                a.gameTimer &&
                a.gameTimerMainScreen &&
                !k.isActive &&
                !y.isActive &&
                !O.isActive &&
                !m.querySelector(`.${De}`)
                    ? t([T.node], yt)
                    : T.node.remove();
            },
            sn = (t = !0) => {
                if (!t) {
                    clearInterval(T.timer), (T.timer = null), Zo([he]), (T.isPaused = !1), (T.pausedTime = 0);
                    return;
                }
                let s = b(he);
                s || ((s = To()), c(he, s)),
                    (T.isPaused = s.isPaused),
                    (T.pausedTime = s.pausedTime),
                    (T.node = e(["game-timer"])),
                    s.isPaused && n(T.node, "paused"),
                    s.endedByWin && n(T.node, "ended");
                const i = () => {
                    const t = b(he);
                    if (!t) return;
                    const s = t.startTime;
                    let e;
                    t.isPaused ? (e = Math.floor((t.pausedTime - s) / 1e3)) : (e = Math.floor((Date.now() - s) / 1e3));
                    const o = Math.floor(e / 3600),
                        i = Math.floor((e % 3600) / 60),
                        a = e % 60;
                    let n = "";
                    o > 0 && (n = `${o.toString().padStart(2, "0")}:`),
                        (n += `${i.toString().padStart(2, "0")}:${a.toString().padStart(2, "0")}`),
                        (T.node.textContent = n);
                };
                i(),
                    (T.timer = setInterval(i, 1e3)),
                    T.node.addEventListener(o, () => {
                        const e = b(he);
                        if (e.isPaused) {
                            const t = Date.now() - e.pausedTime;
                            (e.startTime += t),
                                (e.isPaused = !1),
                                (e.pausedTime = 0),
                                (e.endedByWin = !1),
                                r(T.node, "paused"),
                                r(T.node, "ended");
                        } else (e.isPaused = !0), (e.pausedTime = Date.now()), n(T.node, "paused");
                        (T.isPaused = e.isPaused), (T.pausedTime = e.pausedTime), c(he, e), i();
                    });
            },
            Ki = () => {
                if (!T.timer) return;
                const e = To();
                (T.isPaused = !1),
                    (T.pausedTime = 0),
                    r(T.node, "paused"),
                    c(he, e),
                    T.node && (T.node.textContent = "00:00");
            },
            Vn = (t = !0) => {
                if ((clearInterval(ne.timer), (ne.timer = null), ne.node && (ne.node.remove(), (ne.node = null)), !t))
                    return;
                ne.node = e(["game-timer", "current-time-clock"]);
                const n = () => {
                    const e = new Date();
                    ne.node.textContent = e.toLocaleTimeString([], {
                        hour: "2-digit",
                        minute: "2-digit",
                        hour12: !a.clockFormat24h,
                    });
                };
                n(), (ne.timer = setInterval(n, 1e3));
            },
            ye = () => {
                if (!ne.node) return;
                const e = k.isActive || y.isActive || O.isActive || m.querySelector(`.${De}`);
                if (!pe || !a.showClockOnMainScreen || e) {
                    ne.node.remove();
                    return;
                }
                const s = a.gameTimer && a.gameTimerMainScreen;
                s ? n(ne.node, "with-game-timer") : r(ne.node, "with-game-timer"), yt && t([ne.node], yt);
            },
            Ne = () =>
                ([1e7] + -1e3 + -4e3 + -8e3 + -1e11).replace(/[018]/g, (e) =>
                    (e ^ (crypto.getRandomValues(new Uint8Array(1))[0] & (15 >> (e / 4)))).toString(16)
                ),
            Ui = () => eo.map((e) => ({ m: e, a: !0, e: !1, id: Ne() })),
            ri = () => Vs.map((e) => ({ m: e, a: !0, e: !1, id: Ne() })),
            Wi = () => Ss.map((e) => ({ m: e, a: !0, e: !1, id: Ne() })),
            $i = () => lo.map((e) => ({ m: e, a: !0, e: !1, id: Ne() })),
            Vi = () => {
                const j = e(["edit-victory-messages-overlay"]),
                    C = e(["close-edit-victory-messages-overlay"]),
                    v = e(["wrapper"]),
                    y = e(["victory-messages-list"]),
                    u = e(["toggle-default-messages"]),
                    O = e(["add-message-wrapper"]),
                    w = e(["add-message-input-wrapper"]),
                    l = e([], M),
                    h = e(["add-message-btn", V]),
                    E = b(En),
                    a = E || ri(),
                    g = () => c(En, a),
                    _ = (e) => a.findIndex((t) => t.id === e),
                    x = (r) => {
                        const { id: d, m: h, a: b, e: m } = r,
                            l = e(["message"]),
                            u = e(["toggle"]),
                            c = e(["message-input"], m ? M : "div"),
                            v = e(["delete"]);
                        (l.dataset.id = d),
                            t([u, c], l),
                            t([l], y),
                            b && n(u),
                            m
                                ? (t([v], l),
                                  n(l, "editable"),
                                  h !== "" && (c.value = h),
                                  (c.placeholder = "Enter a victory message"),
                                  (c.type = p),
                                  (c.autocomplete = "off"),
                                  c.addEventListener(M, () => {
                                      const e = _(d);
                                      e !== -1 && ((a[e].m = c.value), g());
                                  }),
                                  v.addEventListener(o, () => {
                                      const e = _(d);
                                      e !== -1 && (a.splice(e, 1), g(), l.remove()), i("Delete Victory Message");
                                  }))
                                : s(c, h),
                            u.addEventListener(o, () => {
                                f(u);
                                const e = _(d);
                                e !== -1 && ((a[e].a = !a[e].a), g()), i("Toggle Victory Message");
                            });
                    };
                a.forEach((e) => x(e)),
                    C.addEventListener(o, () => P(j)),
                    (l.placeholder = "Add your own message"),
                    (l.type = p),
                    l.addEventListener(M, () => {
                        const e = l.value;
                        e !== "" ? n(h) : r(h);
                    }),
                    h.addEventListener(o, () => {
                        a.push({ m: l.value, a: !0, e: !0, id: Ne() }),
                            x(a[a.length - 1]),
                            g(),
                            (l.value = ""),
                            r(h),
                            v.scrollTo({ top: v.scrollHeight, behavior: "smooth" }),
                            i("Add Custom Victory Message");
                    });
                let d = a.filter((e) => !e.e).every((e) => !e.a);
                return (
                    d || n(u),
                    u.addEventListener(o, () => {
                        f(u),
                            a.forEach((e) => {
                                if (!e.e) {
                                    e.a = d;
                                    const t = y.querySelector(`[data-id="${e.id}"] .toggle`);
                                    t && (d ? n(t) : r(t));
                                }
                            }),
                            (d = !d),
                            g(),
                            i("Toggle Default Victory Messages");
                    }),
                    s(u, "Toggle Default Messages"),
                    t([l, h], w),
                    t([w], O),
                    t([y, u], v),
                    t([v, O, C], j),
                    t([j], m.body),
                    i("Edit Victory Messages Overlay", F),
                    j
                );
            },
            Bi = async () => {
                const g = e(["edit-planechase-cards-overlay"]),
                    E = e(["close-edit-planechase-cards-overlay"]),
                    C = e(["wrapper"]),
                    j = e(["planechase-sets-container"]),
                    w = e(["toggle-button-wrapper"]),
                    h = e(["toggle-default-messages"]),
                    y = e(["cards-loading-banner"]),
                    l = await an(_s, "planar");
                if (!l || l.length === 0) return;
                let x = 0;
                const A = l.length,
                    O = () => {
                        x++, x >= A && n(y, "hidden");
                    };
                y.innerHTML = "Downloading all Planechase cards.<br>This could take a while.";
                const u = b(Bt);
                let a;
                if (u) {
                    const e = l.some((e) => e.id in u);
                    e
                        ? (a = l.reduce((e, t) => ((e[t.id] = u[t.id] === void 0 || u[t.id]), e), {}))
                        : (a = l.reduce((e, t) => ((e[t.id] = !0), e), {}));
                } else a = l.reduce((e, t) => ((e[t.id] = !0), e), {});
                const v = (e = !0) => {
                    c(Bt, a), e && ((k.history = []), (k.currentPlane = null), c(Qe, k));
                };
                (!u || !l.some((e) => e.id in u)) && v(!1);
                const p = l.reduce((e, t) => (e[t.set] || (e[t.set] = []), e[t.set].push(t), e), {}),
                    S = (s, c) => {
                        const l = e(["planechase-card-entry"]),
                            h = e(["toggle"]),
                            u = e(["card-image-container", "loading"]),
                            d = e([], "img");
                        return (
                            (l.dataset.id = s.id),
                            (d.src = s.image),
                            (d.alt = s.type_line || "Planechase card"),
                            d.addEventListener("load", () => {
                                r(u, "loading"), O();
                            }),
                            d.addEventListener("error", () => {
                                r(u, "loading"), O();
                            }),
                            a[s.id] ? n(h) : n(l, I),
                            l.addEventListener(o, () => {
                                f(h), f(l, I), (a[s.id] = !a[s.id]), v(), c && c(), i("Toggle Planechase Card");
                            }),
                            t([d], u),
                            t([h, u], l),
                            l
                        );
                    },
                    M = Object.keys(p).sort((e, t) => {
                        const n = p[e][0]?.set_name || e.toUpperCase(),
                            s = p[t][0]?.set_name || t.toUpperCase();
                        return n.localeCompare(s);
                    });
                M.forEach((c) => {
                    const d = p[c],
                        x = e(["planechase-set-section"]),
                        O = e(["planechase-set-header"]),
                        g = e(["set-header-clickable"]),
                        b = e(["set-expand-btn"]),
                        w = e(["set-name"]),
                        y = e(["set-card-count"]),
                        u = e(["toggle", "set-toggle"]),
                        l = e(["planechase-cards-list", "collapsed"]),
                        C = d[0]?.set_name || c.toUpperCase();
                    s(w, C);
                    const E = d.length,
                        h = () => {
                            const e = d.filter((e) => a[e.id]).length;
                            s(y, `${e}/${E}`);
                        };
                    h();
                    const k = d.every((e) => a[e.id]);
                    k && n(u),
                        g.addEventListener(o, () => {
                            const e = _(l, "collapsed");
                            m.querySelectorAll(".planechase-set-section").forEach((e) => {
                                const t = e.querySelector(".planechase-cards-list"),
                                    s = e.querySelector(".set-expand-btn");
                                t && t !== l && (n(t, "collapsed"), r(s, "expanded"));
                            }),
                                e ? (r(l, "collapsed"), n(b, "expanded")) : (n(l, "collapsed"), r(b, "expanded")),
                                i("Toggle Planechase Set Expansion");
                        }),
                        u.addEventListener(o, (e) => {
                            e.stopPropagation(), f(u);
                            const t = _(u);
                            d.forEach((e) => {
                                a[e.id] = t;
                                const s = l.querySelector(`[data-id="${e.id}"]`),
                                    o = s?.querySelector(".toggle");
                                s && o && (t ? n(o) : r(o), t ? r(s, I) : n(s, I));
                            }),
                                h(),
                                v(),
                                i("Toggle Planechase Set");
                        }),
                        d.forEach((e) => {
                            const n = S(e, h);
                            t([n], l);
                        }),
                        t([b, w, y], g),
                        t([g, u], O),
                        t([O, l], x),
                        t([x], j);
                }),
                    E.addEventListener(o, () => P(g));
                let d = Object.values(a).every((e) => e);
                return (
                    d && n(h),
                    h.addEventListener(o, () => {
                        f(h),
                            (d = !d),
                            l.forEach((e) => {
                                a[e.id] = d;
                                const t = j.querySelector(`[data-id="${e.id}"]`),
                                    s = t?.querySelector(".toggle");
                                t && s && (d ? n(s) : r(s), d ? r(t, I) : n(t, I));
                            }),
                            m.querySelectorAll(".set-toggle").forEach((e) => {
                                d ? n(e) : r(e);
                            }),
                            m.querySelectorAll(".set-card-count").forEach((e) => {
                                const n = e.closest(".planechase-set-section"),
                                    t = n.querySelectorAll(".planechase-card-entry"),
                                    o = d ? t.length : 0;
                                s(e, `${o}/${t.length}`);
                            }),
                            v(),
                            i("Toggle All Planechase Cards");
                    }),
                    s(h, "Toggle All Cards"),
                    t([h], w),
                    t([y, j], C),
                    t([C, w, E], g),
                    t([g], m.body),
                    i("Edit Planechase Cards Overlay", F),
                    g
                );
            },
            Pi = async () => {
                const g = e(["edit-archenemy-cards-overlay"]),
                    k = e(["close-edit-archenemy-cards-overlay"]),
                    E = e(["wrapper"]),
                    j = e(["archenemy-sets-container"]),
                    O = e(["toggle-button-wrapper"]),
                    h = e(["toggle-default-messages"]),
                    w = e(["cards-loading-banner"]),
                    l = await an(fs, "scheme");
                if (!l || l.length === 0) return;
                let C = 0;
                const A = l.length,
                    x = () => {
                        C++, C >= A && n(w, "hidden");
                    };
                w.innerHTML = "Downloading all Archenemy cards.<br>This could take a while.";
                const u = b(Wt);
                let a;
                if (u) {
                    const e = l.some((e) => e.id in u);
                    e
                        ? (a = l.reduce((e, t) => ((e[t.id] = u[t.id] === void 0 || u[t.id]), e), {}))
                        : (a = l.reduce((e, t) => ((e[t.id] = !0), e), {}));
                } else a = l.reduce((e, t) => ((e[t.id] = !0), e), {});
                const v = (e = !0) => {
                    c(Wt, a), e && ((y.history = []), (y.currentScheme = null), (y.ongoingSchemes = []), c(Ke, y));
                };
                (!u || !l.some((e) => e.id in u)) && v(!1);
                const p = l.reduce((e, t) => (e[t.set] || (e[t.set] = []), e[t.set].push(t), e), {}),
                    S = (s, c) => {
                        const l = e(["archenemy-card-entry"]),
                            h = e(["toggle"]),
                            u = e(["card-image-container", "loading"]),
                            d = e([], "img");
                        return (
                            (l.dataset.id = s.id),
                            (d.src = s.image),
                            (d.alt = s.type_line || "Archenemy card"),
                            d.addEventListener("load", () => {
                                r(u, "loading"), x();
                            }),
                            d.addEventListener("error", () => {
                                r(u, "loading"), x();
                            }),
                            a[s.id] ? n(h) : n(l, I),
                            l.addEventListener(o, () => {
                                f(h), f(l, I), (a[s.id] = !a[s.id]), v(), c && c(), i("Toggle Archenemy Card");
                            }),
                            t([d], u),
                            t([h, u], l),
                            l
                        );
                    },
                    M = Object.keys(p).sort((e, t) => {
                        const n = p[e][0]?.set_name || e.toUpperCase(),
                            s = p[t][0]?.set_name || t.toUpperCase();
                        return n.localeCompare(s);
                    });
                M.forEach((c) => {
                    const d = p[c],
                        x = e(["archenemy-set-section"]),
                        O = e(["archenemy-set-header"]),
                        g = e(["set-header-clickable"]),
                        b = e(["set-expand-btn"]),
                        w = e(["set-name"]),
                        y = e(["set-card-count"]),
                        u = e(["toggle", "set-toggle"]),
                        l = e(["archenemy-cards-list", "collapsed"]),
                        C = d[0]?.set_name || c.toUpperCase();
                    s(w, C);
                    const E = d.length,
                        h = () => {
                            const e = d.filter((e) => a[e.id]).length;
                            s(y, `${e}/${E}`);
                        };
                    h();
                    const k = d.every((e) => a[e.id]);
                    k && n(u),
                        g.addEventListener(o, () => {
                            const e = _(l, "collapsed");
                            m.querySelectorAll(".archenemy-set-section").forEach((e) => {
                                const t = e.querySelector(".archenemy-cards-list"),
                                    s = e.querySelector(".set-expand-btn");
                                t && t !== l && (n(t, "collapsed"), r(s, "expanded"));
                            }),
                                e ? (r(l, "collapsed"), n(b, "expanded")) : (n(l, "collapsed"), r(b, "expanded")),
                                i("Toggle Archenemy Set Expansion");
                        }),
                        u.addEventListener(o, (e) => {
                            e.stopPropagation(), f(u);
                            const t = _(u);
                            d.forEach((e) => {
                                a[e.id] = t;
                                const s = l.querySelector(`[data-id="${e.id}"]`),
                                    o = s?.querySelector(".toggle");
                                s && o && (t ? n(o) : r(o), t ? r(s, I) : n(s, I));
                            }),
                                h(),
                                v(),
                                i("Toggle Archenemy Set");
                        }),
                        d.forEach((e) => {
                            const n = S(e, h);
                            t([n], l);
                        }),
                        t([b, w, y], g),
                        t([g, u], O),
                        t([O, l], x),
                        t([x], j);
                }),
                    k.addEventListener(o, () => P(g));
                let d = Object.values(a).every((e) => e);
                return (
                    d && n(h),
                    h.addEventListener(o, () => {
                        f(h),
                            (d = !d),
                            l.forEach((e) => {
                                a[e.id] = d;
                                const t = j.querySelector(`[data-id="${e.id}"]`),
                                    s = t?.querySelector(".toggle");
                                t && s && (d ? n(s) : r(s), d ? r(t, I) : n(t, I));
                            }),
                            m.querySelectorAll(".set-toggle").forEach((e) => {
                                d ? n(e) : r(e);
                            }),
                            m.querySelectorAll(".set-card-count").forEach((e) => {
                                const n = e.closest(".archenemy-set-section"),
                                    t = n.querySelectorAll(".archenemy-card-entry"),
                                    o = d ? t.length : 0;
                                s(e, `${o}/${t.length}`);
                            }),
                            v(),
                            i("Toggle All Archenemy Cards");
                    }),
                    s(h, "Toggle All Cards"),
                    t([h], O),
                    t([w, j], E),
                    t([E, O, k], g),
                    t([g], m.body),
                    i("Edit Archenemy Cards Overlay", F),
                    g
                );
            },
            Di = async () => {
                const u = e(["edit-bounty-cards-overlay"]),
                    L = e(["close-edit-bounty-cards-overlay"]),
                    R = e(["wrapper"]),
                    k = e(["bounty-sets-container"]),
                    A = e(["toggle-button-wrapper"]),
                    p = e(["toggle-default-messages"]),
                    _ = e(["cards-loading-banner"]),
                    a = await So();
                if (!a || a.length === 0) return;
                let z = 0;
                const B = a.length,
                    M = () => {
                        z++, z >= B && n(_, "hidden");
                    };
                _.innerHTML = "Downloading all Bounty cards.<br>This could take a while.";
                const h = b(qt);
                let l;
                if (h) {
                    const e = a.some((e) => e.id in h);
                    e
                        ? (l = a.reduce((e, t) => ((e[t.id] = h[t.id] === void 0 || h[t.id]), e), {}))
                        : (l = a.reduce((e, t) => ((e[t.id] = !0), e), {}));
                } else l = a.reduce((e, t) => ((e[t.id] = !0), e), {});
                const g = (e = !0) => {
                    c(qt, l), e && ((O.history = []), (O.currentBounty = null), (O.rewardLevel = 1), c(Be, O));
                };
                (!h || !a.some((e) => e.id in h)) && g(!1);
                const W = (s) => {
                        const a = e(["bounty-card-entry"]),
                            u = e(["toggle"]),
                            d = e(["card-image-container", "loading"]),
                            c = e([], "img");
                        return (
                            (a.dataset.id = s.id),
                            (c.src = s.image),
                            (c.alt = s.type_line || "Bounty card"),
                            c.addEventListener("load", () => {
                                r(d, "loading"), M();
                            }),
                            c.addEventListener("error", () => {
                                r(d, "loading"), M();
                            }),
                            l[s.id] ? n(u) : n(a, I),
                            a.addEventListener(o, () => {
                                f(u), f(a, I), (l[s.id] = !l[s.id]), g(), y(), V(), j(), i("Toggle Bounty Card");
                            }),
                            t([c], d),
                            t([u, d], a),
                            a
                        );
                    },
                    T = e(["bounty-set-section"]),
                    P = e(["bounty-set-header"]),
                    C = e(["set-header-clickable"]),
                    D = e(["set-expand-btn", "expanded"]),
                    N = e(["set-name"]),
                    S = e(["set-card-count"]),
                    d = e(["toggle", "set-toggle"]),
                    v = e(["bounty-cards-list"]);
                s(N, "Outlaws of Thunder Junction - Bounties");
                const H = a.length,
                    y = () => {
                        const e = a.filter((e) => l[e.id]).length;
                        s(S, `${e}/${H}`);
                    };
                y();
                const V = () => {
                        const e = a.every((e) => l[e.id]);
                        e ? n(d) : r(d);
                    },
                    $ = a.every((e) => l[e.id]);
                $ && n(d),
                    C.addEventListener(o, () => {
                        f(v, "collapsed"), f(D, "expanded");
                    }),
                    d.addEventListener(o, (e) => {
                        e.stopPropagation();
                        const s = a.every((e) => l[e.id]),
                            t = !s;
                        a.forEach((e) => {
                            l[e.id] = t;
                        }),
                            g(),
                            v.querySelectorAll(".bounty-card-entry").forEach((e) => {
                                const s = e.querySelector(".toggle");
                                t ? (n(s), r(e, I)) : (r(s), n(e, I));
                            }),
                            t ? n(d) : r(d),
                            y(),
                            j(),
                            i("Toggle All Bounty Set Cards");
                    }),
                    a.forEach((e) => {
                        const n = W(e);
                        t([n], v);
                    }),
                    t([D, N], C),
                    t([C, S, d], P),
                    t([P, v], T),
                    t([T], k);
                const j = () => {
                    const e = a.every((e) => l[e.id]);
                    e ? n(p) : r(p);
                };
                return (
                    j(),
                    p.addEventListener(o, () => {
                        const t = a.every((e) => l[e.id]),
                            e = !t;
                        a.forEach((t) => {
                            l[t.id] = e;
                        }),
                            g(),
                            k.querySelectorAll(".bounty-card-entry").forEach((t) => {
                                const s = t.querySelector(".toggle");
                                e ? (n(s), r(t, I)) : (r(s), n(t, I));
                            }),
                            j(),
                            i("Toggle All Bounty Cards");
                    }),
                    L.addEventListener(o, () => {
                        n(u, E), u.addEventListener(x, () => u.remove(), w);
                    }),
                    s(p, "Toggle All Cards"),
                    t([p], A),
                    t([_, k], R),
                    t([R, A, L], u),
                    t([u], m.body),
                    i("Edit Bounty Cards Overlay", F),
                    u
                );
            },
            Ai = () => {
                const _ = e(["edit-defeat-messages-overlay"]),
                    C = e(["close-edit-defeat-messages-overlay"]),
                    v = e(["wrapper"]),
                    j = e(["defeat-messages-list"]),
                    h = e(["toggle-default-messages"]),
                    O = e(["add-message-wrapper"]),
                    w = e(["add-message-input-wrapper"]),
                    l = e([], M),
                    m = e(["add-message-btn", V]),
                    E = b(kn),
                    a = E || Ui(),
                    g = () => c(kn, a),
                    y = (e) => a.findIndex((t) => t.id === e),
                    x = (r) => {
                        const { id: d, m: h, a: b, e: m } = r,
                            l = e(["message"]),
                            u = e(["toggle"]),
                            c = e(["message-input"], m ? M : "div"),
                            v = e(["delete"]);
                        (l.dataset.id = d),
                            t([u, c], l),
                            t([l], j),
                            b && n(u),
                            m
                                ? (t([v], l),
                                  n(l, "editable"),
                                  h !== "" && (c.value = h),
                                  (c.placeholder = "Enter a defeat message"),
                                  (c.type = p),
                                  (c.autocomplete = "off"),
                                  c.addEventListener(M, () => {
                                      const e = y(d);
                                      e !== -1 && ((a[e].m = c.value), g());
                                  }),
                                  v.addEventListener(o, () => {
                                      const e = y(d);
                                      e !== -1 && (a.splice(e, 1), g(), l.remove()), i("Delete Defeat Message");
                                  }))
                                : s(c, h),
                            u.addEventListener(o, () => {
                                f(u);
                                const e = y(d);
                                e !== -1 && ((a[e].a = !a[e].a), g()), i("Toggle Defeat Message");
                            });
                    };
                a.forEach((e) => x(e)),
                    C.addEventListener(o, () => P(_)),
                    (l.placeholder = "Add your own message"),
                    (l.type = p),
                    l.addEventListener(M, () => {
                        const e = l.value;
                        e !== "" ? n(m) : r(m);
                    }),
                    m.addEventListener(o, () => {
                        a.push({ m: l.value, a: !0, e: !0, id: Ne() }),
                            x(a[a.length - 1]),
                            g(),
                            (l.value = ""),
                            r(m),
                            v.scrollTo({ top: v.scrollHeight, behavior: "smooth" }),
                            i("Add Custom Defeat Message");
                    });
                let u = a.filter((e) => !e.e).every((e) => !e.a);
                u || n(h),
                    s(h, "Toggle Default Messages"),
                    h.addEventListener(o, () => {
                        f(h),
                            a.forEach((e) => {
                                if (!e.e) {
                                    e.a = u;
                                    const t = j.querySelector(`[data-id="${e.id}"] .toggle`);
                                    t && (u ? n(t) : r(t));
                                }
                            }),
                            (u = !u),
                            g(),
                            i("Toggle Default Defeat Messages");
                    }),
                    t([l, m], w),
                    t([w], O),
                    t([j, h], v),
                    t([v, O, C], _),
                    t([_], d),
                    i("Edit Defeat Messages Overlay", F);
            },
            ki = () => {
                const _ = e(["edit-defeat-messages-overlay"]),
                    C = e(["close-edit-defeat-messages-overlay"]),
                    v = e(["wrapper"]),
                    j = e(["defeat-messages-list"]),
                    h = e(["toggle-default-messages"]),
                    O = e(["add-message-wrapper"]),
                    w = e(["add-message-input-wrapper"]),
                    l = e([], M),
                    m = e(["add-message-btn", V]),
                    E = b(xn),
                    a = E || Wi(),
                    g = () => c(xn, a),
                    y = (e) => a.findIndex((t) => t.id === e),
                    x = (r) => {
                        const { id: d, m: h, a: b, e: m } = r,
                            l = e(["message"]),
                            u = e(["toggle"]),
                            c = e(["message-input"], m ? M : "div"),
                            v = e(["delete"]);
                        (l.dataset.id = d),
                            t([u, c], l),
                            t([l], j),
                            b && n(u),
                            m
                                ? (t([v], l),
                                  n(l, "editable"),
                                  h !== "" && (c.value = h),
                                  (c.placeholder = "Enter a commander defeat message"),
                                  (c.type = p),
                                  (c.autocomplete = "off"),
                                  c.addEventListener(M, () => {
                                      const e = y(d);
                                      e !== -1 && ((a[e].m = c.value), g());
                                  }),
                                  v.addEventListener(o, () => {
                                      const e = y(d);
                                      e !== -1 && (a.splice(e, 1), g(), l.remove()),
                                          i("Delete Commander Defeat Message");
                                  }))
                                : s(c, h),
                            u.addEventListener(o, () => {
                                f(u);
                                const e = y(d);
                                e !== -1 && ((a[e].a = !a[e].a), g()), i("Toggle Commander Defeat Message");
                            });
                    };
                a.forEach((e) => x(e)),
                    C.addEventListener(o, () => P(_)),
                    (l.placeholder = "Add your own message"),
                    (l.type = p),
                    l.addEventListener(M, () => {
                        const e = l.value;
                        e !== "" ? n(m) : r(m);
                    }),
                    m.addEventListener(o, () => {
                        a.push({ m: l.value, a: !0, e: !0, id: Ne() }),
                            x(a[a.length - 1]),
                            g(),
                            (l.value = ""),
                            r(m),
                            v.scrollTo({ top: v.scrollHeight, behavior: "smooth" }),
                            i("Add Custom Commander Defeat Message");
                    });
                let u = a.filter((e) => !e.e).every((e) => !e.a);
                u || n(h),
                    s(h, "Toggle Default Messages"),
                    h.addEventListener(o, () => {
                        f(h),
                            a.forEach((e) => {
                                if (!e.e) {
                                    e.a = u;
                                    const t = j.querySelector(`[data-id="${e.id}"] .toggle`);
                                    t && (u ? n(t) : r(t));
                                }
                            }),
                            (u = !u),
                            g(),
                            i("Toggle Default Commander Defeat Messages");
                    }),
                    t([l, m], w),
                    t([w], O),
                    t([j, h], v),
                    t([v, O, C], _),
                    t([_], d),
                    i("Edit Commander Defeat Messages Overlay", F);
            },
            Ei = () => {
                const _ = e(["edit-defeat-messages-overlay"]),
                    C = e(["close-edit-defeat-messages-overlay"]),
                    v = e(["wrapper"]),
                    j = e(["defeat-messages-list"]),
                    h = e(["toggle-default-messages"]),
                    O = e(["add-message-wrapper"]),
                    w = e(["add-message-input-wrapper"]),
                    l = e([], M),
                    m = e(["add-message-btn", V]),
                    E = b(_n),
                    a = E || $i(),
                    g = () => c(_n, a),
                    y = (e) => a.findIndex((t) => t.id === e),
                    x = (r) => {
                        const { id: d, m: h, a: b, e: m } = r,
                            l = e(["message"]),
                            u = e(["toggle"]),
                            c = e(["message-input"], m ? M : "div"),
                            v = e(["delete"]);
                        (l.dataset.id = d),
                            t([u, c], l),
                            t([l], j),
                            b && n(u),
                            m
                                ? (t([v], l),
                                  n(l, "editable"),
                                  h !== "" && (c.value = h),
                                  (c.placeholder = "Enter a poison defeat message"),
                                  (c.type = p),
                                  (c.autocomplete = "off"),
                                  c.addEventListener(M, () => {
                                      const e = y(d);
                                      e !== -1 && ((a[e].m = c.value), g());
                                  }),
                                  v.addEventListener(o, () => {
                                      const e = y(d);
                                      e !== -1 && (a.splice(e, 1), g(), l.remove()), i("Delete Poison Defeat Message");
                                  }))
                                : s(c, h),
                            u.addEventListener(o, () => {
                                f(u);
                                const e = y(d);
                                e !== -1 && ((a[e].a = !a[e].a), g()), i("Toggle Poison Defeat Message");
                            });
                    };
                a.forEach((e) => x(e)),
                    C.addEventListener(o, () => P(_)),
                    (l.placeholder = "Add your own message"),
                    (l.type = p),
                    l.addEventListener(M, () => {
                        const e = l.value;
                        e !== "" ? n(m) : r(m);
                    }),
                    m.addEventListener(o, () => {
                        a.push({ m: l.value, a: !0, e: !0, id: Ne() }),
                            x(a[a.length - 1]),
                            g(),
                            (l.value = ""),
                            r(m),
                            v.scrollTo({ top: v.scrollHeight, behavior: "smooth" }),
                            i("Add Custom Poison Defeat Message");
                    });
                let u = a.filter((e) => !e.e).every((e) => !e.a);
                u || n(h),
                    s(h, "Toggle Default Messages"),
                    h.addEventListener(o, () => {
                        f(h),
                            a.forEach((e) => {
                                if (!e.e) {
                                    e.a = u;
                                    const t = j.querySelector(`[data-id="${e.id}"] .toggle`);
                                    t && (u ? n(t) : r(t));
                                }
                            }),
                            (u = !u),
                            g(),
                            i("Toggle Default Poison Defeat Messages");
                    }),
                    t([l, m], w),
                    t([w], O),
                    t([j, h], v),
                    t([v, O, C], _),
                    t([_], d),
                    i("Edit Poison Defeat Messages Overlay", F);
            },
            Ze = async () => {
                let $e = !1,
                    Je = h.autoHighroll,
                    ze = !1;
                const re = e(["settings-overlay"]),
                    fe = e(["settings-overlay-list"]),
                    Be = e(["settings-headline"]),
                    l = e(["restart-btn", V]),
                    Ne = e(["close-settings-overlay-btn"]),
                    Qe = (a, u, h) => {
                        const f = e(["custom-life-overlay"]),
                            v = e(["overlay-text"]),
                            m = e([], M),
                            b = e(["btn-wrapper"]),
                            p = e(["confirm", N, V]),
                            g = e(["cancel", N]),
                            j = () => {
                                n(f, E),
                                    f.addEventListener(
                                        x,
                                        () => {
                                            f.remove();
                                        },
                                        w
                                    );
                            };
                        g.addEventListener(o, () => {
                            j();
                        }),
                            p.addEventListener(o, () => {
                                const e = m.value;
                                if (e)
                                    return (
                                        u.forEach((e) => r(e)),
                                        a === "startingLife2P"
                                            ? ((Se = "life" + e), c(Vt, Se))
                                            : a === "startingLifeMP" && ((Ue = "life" + e), c(Ht, Ue)),
                                        cn(),
                                        n(h),
                                        s(h, e),
                                        r(h, "custom"),
                                        r(l, V),
                                        i("Set Custom Starting Life"),
                                        j(),
                                        e
                                    );
                            }),
                            m.addEventListener(M, () => {
                                const e = m.value;
                                e && Number.isInteger(Number(e)) ? r(p, V) : n(p, V);
                            }),
                            s(p, "Confirm"),
                            s(g, "Cancel"),
                            s(v, "Enter a custom starting life"),
                            (m.type = "number"),
                            (m.placeholder = 0),
                            t([g, p], b),
                            t([v, m, b], f),
                            t([f], d);
                    },
                    Ve = e(["settings-list-item"]),
                    Le = e(["settings-label"]),
                    Re = e(["settings-label", "in-list-label"]),
                    C = e(["settings-select", p, "planechase"]),
                    ke = e(["settings-select", p, "planechase", "subentry", "edit-planechase-cards"]),
                    b = e(["settings-select", p, "archenemy"]),
                    Oe = e(["settings-select", p, "archenemy", "subentry", "edit-archenemy-cards"]),
                    v = e(["settings-select", p, "bounty"]),
                    _e = e(["settings-select", p, "bounty", "subentry", "edit-bounty-cards"]),
                    K = e(["settings-select", p, "turn-time-tracker-entry", "turn-tracker-main"]),
                    m = e(["settings-select", p, "turn-time-tracker-entry"]),
                    Y = e(["settings-select", p, "turn-time-tracker-entry", "subentry"]),
                    G = e(["settings-select", p, "game-timer-entry"]),
                    Q = e(["settings-select", p, "game-timer-entry", "subentry"]),
                    Z = e(["settings-select", p, "clock-entry"]),
                    J = e(["settings-select", p, "clock-entry", "subentry"]),
                    ee = e(["settings-select", p]),
                    te = e(["settings-select", p]),
                    ne = e(["settings-select", p, "show-player-names-on-card-setting"]),
                    oe = e(["settings-select", p, "show-counters-on-card-setting"]),
                    ie = e(["settings-select", p, "subentry", "show-regular-counters-setting"]),
                    ae = e(["settings-select", p, "subentry", "subsubentry", "keep-zero-counters-setting"]),
                    ve = e(["settings-select", p, "subentry", "subsubentry", "persistent-counters-setting"]),
                    S = e(["settings-select", p, "subentry", "show-commander-damage-setting"]),
                    q = e(["settings-select", p, "subentry", "subsubentry", "clickable-commander-damage-setting"]),
                    A = e(["settings-select", p, "salty-defeat-entry"]),
                    U = e(["settings-select", p, "salty-defeat-entry", "subentry"]),
                    T = e(["settings-select", p, "random-player-colors-entry"]),
                    D = e(["settings-select", p, "random-player-colors-entry", "subentry"]),
                    W = e(["settings-select", p]),
                    $ = e(["settings-select", p]),
                    B = e(["settings-select", p]),
                    I = e(["settings-select", p]),
                    H = e(["settings-select", p]),
                    P = e(["settings-select", p, "custom-long-tap-entry"]),
                    he = e(["settings-select", p, "custom-long-tap-entry", "subentry"]),
                    R = e(["settings-select", p, "custom-single-tap-entry"]),
                    de = e(["settings-select", p, "custom-single-tap-entry", "subentry"]),
                    De = e(["settings-list-item"]),
                    Fe = e(["settings-label"]),
                    u = e(["settings-select", p, "whitelabel-icon"]),
                    be = e(["settings-select", p, "edit-profiles"]),
                    je = e(["settings-select", p, "edit-defeat-messages"]),
                    Me = e(["settings-select", p, "edit-commander-defeat-messages"]),
                    Ee = e(["settings-select", p, "edit-poison-defeat-messages"]),
                    Ce = e(["settings-select", p, "edit-victory-messages"]),
                    xe = e(["settings-select", p, "reset-player-background-colors"]),
                    we = e(["settings-select", p, "reset-app"]),
                    He = e(["feedback-btn-wrapper"]),
                    ge = e(["feedback-btn"], "a"),
                    Te = e(["patreon-btn-wrapper"]),
                    pe = e(["patreon-btn"], "a"),
                    Ye = e(["icon"]),
                    Ke = e(["text"], "span"),
                    qe = e(["settings-select", p, "build"]),
                    We = () => {
                        f(m),
                            _(m)
                                ? ((h.autoHighroll = !0), i("Auto-Highroll at Game Start: Enable"))
                                : ((h.autoHighroll = !1), i("Auto-Highroll at Game Start: Disable")),
                            r(l, V);
                    },
                    Ge = [
                        { c: h.isActive, e: K },
                        { c: h.turnTimer.isActive, e: Y },
                        { c: h.autoHighroll, e: m },
                        { c: k.isActive, e: C },
                        { c: y.isActive, e: b },
                        { c: O.isActive, e: v },
                        { c: a.autoKO, e: ee },
                        { c: a.lifeLossOnCommanderDamage, e: te },
                        { c: a.showPlayerNamesOnCard, e: ne },
                        { c: a.showCountersOnPlayerCard, e: oe },
                        { c: a.showRegularCounters, e: ie },
                        { c: a.keepZeroCountersOnPlayerCard, e: ae },
                        { c: a.persistentCounters.length > 0, e: ve },
                        { c: a.showCommanderDamageCounters, e: S },
                        { c: a.clickableCommanderDamageCounters, e: q },
                        { c: a.saltyDefeatMessages, e: A },
                        { c: a.cycleSaltyDefeatMessages, e: U },
                        { c: a.randomPlayerColors, e: T },
                        { c: a.preserveBackgroundImagesOnShuffle, e: D },
                        { c: a.setLifeByTappingNumber, e: W },
                        { c: a.verticalTapAreas, e: $ },
                        { c: a.barredDigits, e: I },
                        { c: a.cleanLook, e: B },
                        { c: a.criticalDamageWarning, e: H },
                        { c: a.customLongTapEnabled, e: P },
                        { c: a.customSingleTapEnabled, e: R },
                        { c: a.gameTimer, e: G },
                        { c: a.gameTimerMainScreen, e: Q },
                        { c: a.showClockOnMainScreen, e: Z },
                        { c: a.clockFormat24h, e: J },
                    ];
                Ge.forEach(({ c: e, e: t }) => e && n(t)),
                    K.addEventListener(o, () => {
                        Ls(),
                            f(K),
                            _(K)
                                ? ((h.isActive = !0), _(m) || (n(m), (h.autoHighroll = !0)), i("Turn Tracker: Enable"))
                                : ((h.isActive = !1),
                                  i("Turn Tracker: Disable"),
                                  X && (X.remove(), (X = null)),
                                  Pe !== null && clearInterval(Pe),
                                  Je !== h.autoHighroll && !ze && We()),
                            c(Ie, h),
                            r(l, V),
                            ($e = !0);
                    }),
                    Y.addEventListener(o, () => {
                        f(Y),
                            _(Y)
                                ? ((h.turnTimer.isActive = !0), i("Turn Timer: Enable"))
                                : ((h.turnTimer.isActive = !1), clearInterval(Pe), i("Turn Timer: Disable")),
                            gs(),
                            c(Ie, h),
                            r(l, V),
                            Ut();
                    }),
                    m.addEventListener(o, () => {
                        We(), c(Ie, h), (ze = !0);
                    }),
                    te.addEventListener(o, () => {
                        f(te),
                            _(te)
                                ? ((a.lifeLossOnCommanderDamage = !0), i("Commander Damage: Enable Life Loss"))
                                : ((a.lifeLossOnCommanderDamage = !1), i("Commander Damage: Disable Life Loss")),
                            c(L, a);
                    }),
                    ee.addEventListener(o, () => {
                        f(ee),
                            _(ee)
                                ? ((a.autoKO = !0), i("Auto-Kill: Enable"))
                                : ((a.autoKO = !1), i("Auto-Kill: Disable")),
                            c(L, a);
                    }),
                    ne.addEventListener(o, () => {
                        f(ne),
                            (a.showPlayerNamesOnCard = _(ne)),
                            i(`Player Names on Card: ${a.showPlayerNamesOnCard ? "Enable" : "Disable"}`),
                            c(L, a),
                            j.forEach((e) => os(e));
                    }),
                    oe.addEventListener(o, () => {
                        f(oe),
                            _(oe)
                                ? ((a.showCountersOnPlayerCard = !0), i("Show Counters on Player Card: Enable"))
                                : ((a.showCountersOnPlayerCard = !1), i("Show Counters on Player Card: Disable")),
                            c(L, a),
                            j.forEach((e) => {
                                ue(e), me(e);
                            });
                    }),
                    ie.addEventListener(o, () => {
                        f(ie),
                            _(ie)
                                ? ((a.showRegularCounters = !0), i("Show Regular Counters on Player Card: Enable"))
                                : ((a.showRegularCounters = !1), i("Show Regular Counters on Player Card: Disable")),
                            c(L, a),
                            j.forEach((e) => ue(e));
                    }),
                    ae.addEventListener(o, () => {
                        f(ae),
                            _(ae)
                                ? ((a.keepZeroCountersOnPlayerCard = !0),
                                  i("Keep Zero Counters on Player Card: Enable"))
                                : ((a.keepZeroCountersOnPlayerCard = !1),
                                  i("Keep Zero Counters on Player Card: Disable")),
                            c(L, a),
                            j.forEach((e) => ue(e));
                    }),
                    ve.addEventListener(o, () => Fa()),
                    S.addEventListener(o, () => {
                        f(S),
                            _(S)
                                ? ((a.showCommanderDamageCounters = !0),
                                  i("Show Commander Damage Counters on Player Card: Enable"))
                                : ((a.showCommanderDamageCounters = !1),
                                  i("Show Commander Damage Counters on Player Card: Disable")),
                            c(L, a),
                            j.forEach((e) => me(e));
                    }),
                    q.addEventListener(o, () => {
                        f(q),
                            _(q)
                                ? ((a.clickableCommanderDamageCounters = !0),
                                  n(d, "clickable-cmd-dmg-enabled"),
                                  i("Clickable Commander Damage Counters: Enable"))
                                : ((a.clickableCommanderDamageCounters = !1),
                                  r(d, "clickable-cmd-dmg-enabled"),
                                  i("Clickable Commander Damage Counters: Disable")),
                            c(L, a),
                            j.forEach((e) => me(e));
                    }),
                    A.addEventListener(o, () => {
                        f(A),
                            _(A)
                                ? ((a.saltyDefeatMessages = !0), i("Salty Defeat Messages: Enable"))
                                : ((a.saltyDefeatMessages = !1), i("Salty Defeat Messages: Disable")),
                            c(L, a);
                    }),
                    U.addEventListener(o, () => {
                        f(U),
                            _(U)
                                ? ((a.cycleSaltyDefeatMessages = !0), i("Cycle Salty Defeat Messages: Enable"))
                                : ((a.cycleSaltyDefeatMessages = !1), i("Cycle Salty Defeat Messages: Disable")),
                            c(L, a);
                    }),
                    T.addEventListener(o, () => {
                        f(T),
                            _(T)
                                ? ((a.randomPlayerColors = !0), i("Random Player Colors: Enable"), r(l, V))
                                : ((a.randomPlayerColors = !1), i("Random Player Colors: Disable")),
                            c(L, a);
                    }),
                    D.addEventListener(o, () => {
                        f(D),
                            _(D)
                                ? ((a.preserveBackgroundImagesOnShuffle = !0),
                                  i("Preserve Background Images On Shuffle: Enable"))
                                : ((a.preserveBackgroundImagesOnShuffle = !1),
                                  i("Preserve Background Images On Shuffle: Disable")),
                            c(L, a);
                    }),
                    W.addEventListener(o, () => {
                        f(W),
                            _(W)
                                ? ((a.setLifeByTappingNumber = !0),
                                  n(d, "set-life-by-tap-enabled"),
                                  i("Set Life By Tapping Number: Enable"))
                                : ((a.setLifeByTappingNumber = !1),
                                  r(d, "set-life-by-tap-enabled"),
                                  i("Set Life By Tapping Number: Disable")),
                            c(L, a);
                    }),
                    $.addEventListener(o, () => {
                        f($),
                            _($)
                                ? ((a.verticalTapAreas = !0), i("Vertical Tap Areas: Enable"))
                                : ((a.verticalTapAreas = !1), i("Vertical Tap Areas: Disable")),
                            c(L, a),
                            Xo();
                    }),
                    B.addEventListener(o, () => {
                        f(B),
                            _(B)
                                ? ((a.cleanLook = !0), i("Clean Look: Enable"))
                                : ((a.cleanLook = !1), i("Clean Look: Disable")),
                            c(L, a),
                            Go();
                    }),
                    I.addEventListener(o, () => {
                        f(I),
                            _(I)
                                ? ((a.barredDigits = !0), i("Barred 6 and 9: Enable"))
                                : ((a.barredDigits = !1), i("Barred 6 and 9: Disable")),
                            c(L, a),
                            Vo();
                    }),
                    H.addEventListener(o, () => {
                        f(H),
                            _(H)
                                ? ((a.criticalDamageWarning = !0), i("Low Health Warning: Enable"))
                                : ((a.criticalDamageWarning = !1), i("Low Health Warning: Disable")),
                            c(L, a),
                            j.forEach((e) => it(e));
                    });
                const Xe = [5, 10, 20, "custom"],
                    le = [];
                Xe.forEach((l) => {
                    const u = e(["settings-option"]);
                    if ((s(u, l), l === "custom")) {
                        const e = a.customLongTapValue;
                        [5, 10, 20].includes(e) ? (s(u, ""), n(u, "custom")) : (s(u, e), n(u));
                    } else l === a.customLongTapValue && n(u);
                    le.push(u),
                        t([u], he),
                        u.addEventListener(o, () => {
                            if (l === "custom") {
                                const h = e(["custom-life-overlay"]),
                                    p = e(["overlay-text"]),
                                    l = e([], M),
                                    g = e(["btn-wrapper"]),
                                    m = e(["confirm", N, V]),
                                    f = e(["cancel", N]),
                                    v = () => {
                                        n(h, E), h.addEventListener(x, () => h.remove(), w);
                                    };
                                f.addEventListener(o, v),
                                    m.addEventListener(o, () => {
                                        const e = Number(l.value);
                                        e &&
                                            Number.isInteger(e) &&
                                            e > 0 &&
                                            (le.forEach((e) => r(e)),
                                            (a.customLongTapValue = e),
                                            c(L, a),
                                            n(u),
                                            s(u, e),
                                            r(u, "custom"),
                                            i("Set Custom Long Tap Value: " + e),
                                            v());
                                    }),
                                    l.addEventListener(M, () => {
                                        const e = l.value;
                                        e && Number.isInteger(Number(e)) && Number(e) > 0 ? r(m, V) : n(m, V);
                                    }),
                                    s(m, "Confirm"),
                                    s(f, "Cancel"),
                                    s(p, "Enter a custom long tap value"),
                                    (l.type = "number"),
                                    (l.placeholder = 10),
                                    t([f, m], g),
                                    t([p, l, g], h),
                                    t([h], d);
                            } else {
                                le.forEach((e) => r(e)), (a.customLongTapValue = l), c(L, a), n(u);
                                const e = le[le.length - 1];
                                s(e, ""), n(e, "custom"), i("Set Long Tap Value: " + l);
                            }
                        });
                }),
                    (he.style.display = a.customLongTapEnabled ? "" : "none"),
                    P.addEventListener(o, () => {
                        f(P),
                            _(P)
                                ? ((a.customLongTapEnabled = !0), (he.style.display = ""), i("Custom Long Tap: Enable"))
                                : ((a.customLongTapEnabled = !1),
                                  (he.style.display = "none"),
                                  i("Custom Long Tap: Disable")),
                            c(L, a);
                    });
                const Ze = [1, 2, 5, "custom"],
                    ce = [];
                if (
                    (Ze.forEach((l) => {
                        const u = e(["settings-option"]);
                        if ((s(u, l), l === "custom")) {
                            const e = a.customSingleTapValue;
                            [1, 2, 5].includes(e) ? (s(u, ""), n(u, "custom")) : (s(u, e), n(u));
                        } else l === a.customSingleTapValue && n(u);
                        ce.push(u),
                            t([u], de),
                            u.addEventListener(o, () => {
                                if (l === "custom") {
                                    const h = e(["custom-life-overlay"]),
                                        p = e(["overlay-text"]),
                                        l = e([], M),
                                        g = e(["btn-wrapper"]),
                                        m = e(["confirm", N, V]),
                                        f = e(["cancel", N]),
                                        v = () => {
                                            n(h, E), h.addEventListener(x, () => h.remove(), w);
                                        };
                                    f.addEventListener(o, v),
                                        m.addEventListener(o, () => {
                                            const e = Number(l.value);
                                            e &&
                                                Number.isInteger(e) &&
                                                e > 0 &&
                                                (ce.forEach((e) => r(e)),
                                                (a.customSingleTapValue = e),
                                                c(L, a),
                                                n(u),
                                                s(u, e),
                                                r(u, "custom"),
                                                i("Set Custom Single Tap Value: " + e),
                                                v());
                                        }),
                                        l.addEventListener(M, () => {
                                            const e = l.value;
                                            e && Number.isInteger(Number(e)) && Number(e) > 0 ? r(m, V) : n(m, V);
                                        }),
                                        s(m, "Confirm"),
                                        s(f, "Cancel"),
                                        s(p, "Enter a custom single tap value"),
                                        (l.type = "number"),
                                        (l.placeholder = 1),
                                        t([f, m], g),
                                        t([p, l, g], h),
                                        t([h], d);
                                } else {
                                    ce.forEach((e) => r(e)), (a.customSingleTapValue = l), c(L, a), n(u);
                                    const e = ce[ce.length - 1];
                                    s(e, ""), n(e, "custom"), i("Set Single Tap Value: " + l);
                                }
                            });
                    }),
                    (de.style.display = a.customSingleTapEnabled ? "" : "none"),
                    R.addEventListener(o, () => {
                        f(R),
                            _(R)
                                ? ((a.customSingleTapEnabled = !0),
                                  (de.style.display = ""),
                                  i("Custom Single Tap: Enable"))
                                : ((a.customSingleTapEnabled = !1),
                                  (de.style.display = "none"),
                                  i("Custom Single Tap: Disable")),
                            c(L, a);
                    }),
                    G.addEventListener(o, () => {
                        f(G),
                            _(G)
                                ? ((a.gameTimer = !0), sn(), Ki(), i("Game Timer: Enable"))
                                : ((a.gameTimer = !1), sn(!1), i("Game Timer: Disable")),
                            c(L, a),
                            Ae(),
                            ye();
                    }),
                    Q.addEventListener(o, () => {
                        f(Q),
                            _(Q)
                                ? ((a.gameTimerMainScreen = !0), i("Game Timer on Main Screen: Enable"))
                                : ((a.gameTimerMainScreen = !1), i("Game Timer on Main Screen: Disable")),
                            c(L, a),
                            Ae(),
                            ye();
                    }),
                    Z.addEventListener(o, () => {
                        f(Z),
                            _(Z)
                                ? ((a.showClockOnMainScreen = !0), Vn(), i("Clock on Main Screen: Enable"))
                                : ((a.showClockOnMainScreen = !1), Vn(!1), i("Clock on Main Screen: Disable")),
                            c(L, a),
                            ye();
                    }),
                    J.addEventListener(o, () => {
                        f(J),
                            (a.clockFormat24h = _(J)),
                            c(L, a),
                            i("Use 24-Hour Time: " + (a.clockFormat24h ? "Enable" : "Disable"));
                    }),
                    C.addEventListener(o, () => {
                        f(C), _(C) ? jn() || f(C) : rs(), Ae(), ye();
                    }),
                    ke.addEventListener(o, () => {
                        Bi();
                    }),
                    Oe.addEventListener(o, () => {
                        Pi();
                    }),
                    b.addEventListener(o, () => {
                        f(b), _(b) ? wn() || f(b) : pn(), Ae(), ye();
                    }),
                    _e.addEventListener(o, () => {
                        Di();
                    }),
                    v.addEventListener(o, () => {
                        f(v), _(v) ? On() || f(v) : Cn(), Ae(), ye();
                    }),
                    t([Be], fe),
                    Object.keys(Io).forEach((a) => {
                        const { label: p, options: g } = Io[a],
                            h = e(["settings-list-item"]),
                            m = e(["settings-label", "starting-life-label"]),
                            f = e(["settings-select"]),
                            u = [];
                        s(m, p);
                        let d;
                        a === "startingLife2P" ? (d = Se) : a === "startingLifeMP" && (d = Ue),
                            (d = Ct(d)),
                            g.forEach((h) => {
                                const m = e(["settings-option"]);
                                s(m, h),
                                    h === "custom" &&
                                        (Dn.includes(Number(d)) ? (s(m, ""), n(m, "custom")) : (s(m, d), n(m))),
                                    t([m], f),
                                    u.push(m),
                                    String(h) === String(d) && n(m),
                                    m.addEventListener(o, () => {
                                        h === "custom"
                                            ? Qe(a, u, m)
                                            : (u.forEach((e) => r(e)),
                                              a === "startingLife2P"
                                                  ? ((Se = "life" + h), c(Vt, Se))
                                                  : a === "startingLifeMP" && ((Ue = "life" + h), c(Ht, Ue)),
                                              cn(),
                                              n(m),
                                              r(l, V),
                                              i("Set Starting Life: " + h));
                                    });
                            }),
                            t([m, f], h),
                            t([h], fe);
                    }),
                    l.addEventListener(o, zt),
                    Ne.addEventListener(o, () => {
                        n(re, E),
                            re.addEventListener(
                                x,
                                () => {
                                    re.remove(), $e && h.isActive && Yn();
                                },
                                w
                            ),
                            i("Close Settings Overlay");
                    }),
                    xe.addEventListener(o, () => {
                        dt("Are you sure you want to restore the default background colors?", "Cancel", "Restore").then(
                            (e) => {
                                e && (oa(), i("Reset Player Background Colors"), on());
                            }
                        );
                    }),
                    we.addEventListener(o, () => {
                        dt("Are you sure you want to reset the app and delete all data?", "Cancel", "Reset App").then(
                            (e) => {
                                e && (Da(), i("Reset App"), on());
                            }
                        );
                    }),
                    a.whitelabelIcon !== null)
                ) {
                    const e = await se(a.whitelabelIcon);
                    g("--icon", `url(${e})`, u), n(u);
                }
                u.addEventListener(o, () => {
                    if (a.whitelabelIcon !== null)
                        Wa(a.whitelabelIcon),
                            (a.whitelabelIcon = null),
                            c(L, a),
                            r(u),
                            r(z, "whitelabeled"),
                            i("Remove User Icon");
                    else {
                        const t = e([], M);
                        ks(t, async (e) => {
                            if (e) {
                                (a.whitelabelIcon = e), c(L, a);
                                const t = await se(e);
                                g("--icon", `url(${t})`, u), n(u), g("--icon", `url(${t})`, z), n(z, "whitelabeled");
                            }
                        }),
                            t.click(),
                            i("Upload User Icon");
                    }
                }),
                    be.addEventListener(o, () => Ks(!1)),
                    je.addEventListener(o, () => Ai()),
                    Me.addEventListener(o, () => ki()),
                    Ee.addEventListener(o, () => Ei()),
                    Ce.addEventListener(o, () => Vi()),
                    s(ge, "Got Feedback?"),
                    (ge.href = "https://forms.gle/ykdFrRDkuBJZzqRr7"),
                    (ge.target = "_blank"),
                    s(Ke, "Become a Patreon"),
                    (pe.href = "https://patreon.com/lifecounter"),
                    (pe.target = "_blank"),
                    (K.innerHTML = "<div>Enable Turn Tracker <span>Requires High-roll at Game Start</span></div>"),
                    (Y.innerHTML = "<div>Enable Turn Timer <span>Tracks turn duration</span></div>"),
                    (m.innerHTML =
                        "<div>High-roll at Game Start <span>Auto-highroll and set starting player</span></div>"),
                    s(Be, "Settings"),
                    s(l, "Restart Game"),
                    s(Le, "Game Modes"),
                    s(Re, "Gameplay"),
                    s(C, "Enable Planechase"),
                    (ke.innerHTML = "<div>Edit Planechase Cards <span>Select cards to use</span></div>"),
                    s(b, "Enable Archenemy"),
                    (Oe.innerHTML = "<div>Edit Archenemy Cards <span>Select cards to use</span></div>"),
                    s(v, "Enable Bounty"),
                    (_e.innerHTML = "<div>Edit Bounty Cards <span>Select cards to use</span></div>"),
                    (ee.innerHTML =
                        "<div>Auto-Kill <span>Kill player from life, poison, or commander damage</span></div>"),
                    (te.innerHTML =
                        "<div>Commander Damage <span>Commander damage causes players to lose life</span></div>"),
                    (ne.innerHTML = "<div>Player names on card <span>Show a name on each player card</span></div>"),
                    (oe.innerHTML =
                        "<div>Counters on player card <span>Note: Player card may become cluttered</span></div>"),
                    (ie.innerHTML = "<div>Regular Counters <span>Poison, tax, energy, etc.</span></div>"),
                    (ae.innerHTML =
                        "<div>Keep Zero Counters <span>Keep counters at 0 visible <br>on player card</span></div>"),
                    (ve.innerHTML =
                        "<div>Pinned Counters <span>Chosen counters stay on every card,<br>even between games</span></div>"),
                    (S.innerHTML = "<div>Commander Damage <span>Show received commander damage</span></div>"),
                    (q.innerHTML =
                        "<div>Tap to Adjust <span>Tap a counter to adjust <br>commander damage</span></div>"),
                    s(A, "Salty Defeat Messages"),
                    (U.innerHTML = "<div>Cycle Messages <span>Messages change over time</span></div>"),
                    (T.innerHTML =
                        "<div>Shuffle Player Colors <span>Player cards get random colors on start</span></div>"),
                    (D.innerHTML =
                        "<div>Preserve Background Images <span>Keep custom images when shuffling</span></div>"),
                    (W.innerHTML =
                        "<div>Set Life Total Overlay <span>Tap a player card’s life total to enter a new value</span></div>"),
                    ($.innerHTML =
                        "<div>Vertical Tap Areas <span>Switch increase/decrease area from horizontal <br>(side-by-side) to vertical (top/bottom)</span></div>"),
                    (B.innerHTML =
                        "<div>Minimalist Mode <span>Hide +/– hints for a cleaner player card look</span></div>"),
                    (I.innerHTML =
                        "<div>Underlined 6 and 9 <span>Add a line under 6 and 9 so they’re not confused when read upside down</span></div>"),
                    (H.innerHTML =
                        "<div>Low Health Warning <span>Show blinking red alert when life is below 10</span></div>"),
                    (P.innerHTML =
                        "<div>Custom Long Tap Value <span>Change the default +/– 10 for long presses</span></div>"),
                    (R.innerHTML =
                        "<div>Custom Single Tap Value <span>Change the default +/– 1 for single taps</span></div>"),
                    s(G, "Enable Game Timer"),
                    (Q.innerHTML =
                        "<div>Display on Main Screen <span>Not compatible with Planechase, <br>Archenemy & Bounty game mode</span></div>"),
                    (Z.innerHTML = "<div>Show Current Time <span>Display current time on main screen</span></div>"),
                    (J.innerHTML =
                        "<div>Use 24-Hour Time <span>Show the clock as 24-hour instead of AM/PM</span></div>"),
                    (xe.innerHTML =
                        "<div>Reset Player Backgrounds <span>Restore default player background colors</span></div>"),
                    (we.innerHTML = "<div>Reset App <span>Erase all content and settings</span></div>"),
                    s(qe, Version),
                    s(Fe, "Customize"),
                    (u.innerHTML = "<div>Add Your Logo <span>Swap the menu button with your own icon</span></div>"),
                    (be.innerHTML = "<div>Profiles <span>Edit player profiles</span></div>"),
                    (je.innerHTML = "<div>Defeat Messages</div>"),
                    (Me.innerHTML = "<div>Commander Defeat Messages</div>"),
                    (Ee.innerHTML = "<div>Poison Defeat Messages</div>"),
                    (Ce.innerHTML = "<div>Victory Messages</div>"),
                    t([ge], He),
                    t([Ye, Ke], pe),
                    t([pe], Te),
                    t(
                        [
                            Le,
                            C,
                            ke,
                            b,
                            Oe,
                            v,
                            _e,
                            Re,
                            K,
                            Y,
                            m,
                            G,
                            Q,
                            ee,
                            te,
                            oe,
                            ie,
                            ae,
                            ve,
                            S,
                            q,
                            ne,
                            A,
                            U,
                            T,
                            D,
                            W,
                            $,
                            B,
                            I,
                            H,
                            P,
                            he,
                            R,
                            de,
                            Z,
                            J,
                        ],
                        Ve
                    ),
                    t([Fe, je, Me, Ee, Ce, be, u, xe, we, He, Te, qe], De),
                    t([Ve, De], fe),
                    t([fe, Ne, l], re),
                    t([re], d),
                    i("Settings Overlay", F);
            },
            yi = () => {
                const s = e(["more-overlay"]),
                    r = e(["close-more-overlay-btn"]),
                    c = Nt.innerHTML,
                    l = c.split("<hr>"),
                    u = l[1] || "",
                    a = e();
                (a.innerHTML = u),
                    (a.id = "Content"),
                    r.addEventListener(o, () => {
                        n(s, E),
                            s.addEventListener(
                                x,
                                () => {
                                    s.remove();
                                },
                                w
                            ),
                            i("Close More Overlay");
                    }),
                    t([a, r], s),
                    t([s], d),
                    i("More Overlay", F);
            },
            ji = (e, t = 0, n = Ge) => {
                C(e, "opacity", t), C(e, "animation", n);
            },
            bi = () => {
                i("Open Card Search");
                let l = 0,
                    c;
                const u = e(["card-search-overlay", "menu-card-search"]),
                    a = e(["results-wrapper"]),
                    r = e(["search-input"], M),
                    m = e(["close-card-search-overlay"]),
                    g = (e, t) => {
                        let n;
                        return function (...s) {
                            clearTimeout(n),
                                (n = setTimeout(() => {
                                    e.apply(this, s);
                                }, t));
                        };
                    },
                    y = (n) => {
                        const i = e(["legalities-wrapper"]),
                            o = [
                                "standard",
                                "pioneer",
                                "modern",
                                "legacy",
                                "vintage",
                                "commander",
                                "oathbreaker",
                                "alchemy",
                                "explorer",
                                "historic",
                                "brawl",
                                "timeless",
                                "pauper",
                                "penny",
                            ],
                            a = o.reduce(
                                (i, a) => {
                                    if (n[a]) {
                                        const r = e(["format-item"]),
                                            c = n[a] === "legal",
                                            l = e(["format-status", c ? "legal" : "not-legal"], "span"),
                                            d = e(["format-name"], "span");
                                        s(l, c ? "LEGAL" : "NOT LEGAL"), s(d, a), t([l, d], r);
                                        const u = o.indexOf(a) < o.length / 2 ? 0 : 1;
                                        i[u].push(r);
                                    }
                                    return i;
                                },
                                [[], []]
                            );
                        return (
                            a.forEach((n) => {
                                const s = e(["legalities-column"]);
                                t(n, s), t([s], i);
                            }),
                            i
                        );
                    },
                    j = (n) => {
                        const o = e(["price-wrapper"]);
                        if (n) {
                            if (n.usd || n.eur) {
                                const i = e(["price-item"]),
                                    a = n.usd ? `$${parseFloat(n.usd).toFixed(2)}` : "",
                                    r = n.eur ? `€${parseFloat(n.eur).toFixed(2)}` : "";
                                s(i, [a, r].filter(Boolean).join(" | ")), t([i], o);
                            }
                            if (n.usd_foil || n.eur_foil) {
                                const i = e(["price-item", "foil"]),
                                    a = n.usd_foil ? `$${parseFloat(n.usd_foil).toFixed(2)}` : "",
                                    r = n.eur_foil ? `€${parseFloat(n.eur_foil).toFixed(2)}` : "";
                                s(i, `Foil: ${[a, r].filter(Boolean).join(" | ")}`), t([i], o);
                            }
                            if (n.usd_etched) {
                                const i = e(["price-item", "etched"]);
                                s(i, `Etched-foil: $${parseFloat(n.usd_etched).toFixed(2)}`), t([i], o);
                            }
                        }
                        return o;
                    },
                    v = (n) => {
                        const i = e(["print-wrapper"]),
                            o = e(["card-image"]),
                            a = e(["print-info"]),
                            r = j(n.prices);
                        return (
                            n.image_uris?.large
                                ? C(o, "backgroundImage", `url(${n.image_uris.large})`)
                                : n.card_faces &&
                                  n.card_faces[0].image_uris?.large &&
                                  C(o, "backgroundImage", `url(${n.card_faces[0].image_uris.large})`),
                            s(a, n.set_name),
                            t([o, a, r], i),
                            i
                        );
                    },
                    b = (n) => {
                        a.innerHTML = "";
                        const o = e(["card-image-wrapper"]),
                            i = y(n.legalities);
                        n.prints_search_uri &&
                            fetch(n.prints_search_uri)
                                .then((e) => e.json())
                                .then((e) => {
                                    e.data &&
                                        Array.isArray(e.data) &&
                                        e.data.forEach((e) => {
                                            const n = v(e);
                                            t([n], o);
                                        });
                                })
                                .catch((n) => {
                                    console.error("Error fetching prints:", n);
                                    const i = e(["prints-error"]);
                                    s(i, "Error loading prints"), t([i], o);
                                }),
                            t([o, i], a);
                    },
                    h = (n, s) => {
                        const i = e(["card-image"]);
                        C(i, "backgroundImage", `url(${n})`),
                            t([i], a),
                            i.addEventListener(o, () => {
                                b(s);
                            });
                    },
                    f = (n, o) => {
                        if (!n || !Array.isArray(n.data)) {
                            const n = e(["no-results"]);
                            s(n, $t[Le(0, $t.length - 1)]), t([n], a);
                            return;
                        }
                        const i = n.data,
                            r = i.slice(l, l + 5);
                        if (
                            (r.forEach((e) => {
                                const n = !o && e.prints_search_uri,
                                    t = "large";
                                e.image_uris && e.image_uris[t]
                                    ? h(e.image_uris[t], e, n)
                                    : e.card_faces &&
                                      Array.isArray(e.card_faces) &&
                                      e.card_faces.forEach((s) => {
                                          s.image_uris && s.image_uris[t] && h(s.image_uris[t], e, n);
                                      });
                            }),
                            (l += 5),
                            l < i.length)
                        ) {
                            const e = a.lastElementChild;
                            e && c.observe(e);
                        }
                    },
                    _ = (e, t = !1) => {
                        ot(a),
                            (l = 0),
                            c && c.disconnect(),
                            (c = new IntersectionObserver(
                                (n) => {
                                    n[0].isIntersecting && (c.unobserve(n[0].target), f(e, t));
                                },
                                { root: a, rootMargin: "0px", threshold: 1 }
                            )),
                            f(e, t);
                    },
                    w = () => {
                        const e = r.value.trim();
                        if (e.length < 3) return;
                        fetch(`${dn}order=released&q=${e}`)
                            .then((e) => e.json())
                            .then((e) => {
                                _(e), n(u, "active-results");
                            });
                    };
                (r.type = p),
                    (r.placeholder = "Search for a card"),
                    (r.autocomplete = "off"),
                    r.addEventListener(M, g(w, 500)),
                    m.addEventListener(o, () => P(u)),
                    t([a, r, m], u),
                    t([u], d);
            },
            Mn = "iOS",
            Sn = "Android",
            pi = () => {
                const e = navigator.userAgent.toLowerCase();
                return /ipad|iphone|ipod|macintosh.*mobile/.test(e) ? Mn : /android/.test(e) ? Sn : "Other";
            },
            ui = (n) => {
                const i = e(["patreon"], "a"),
                    u = e(["icon"]),
                    l = e([p]),
                    d = e([N]),
                    r = e(["close"]),
                    h = e(["x-icon"]),
                    m = b(In),
                    f = b(Rn),
                    g = b(ni),
                    v = pe ? f : m,
                    j = v && !g;
                s(l, "Become a Patron!"),
                    s(d, "Support"),
                    (i.href = "https://patreon.com/lifecounter"),
                    (i.target = "_blank"),
                    r.addEventListener(o, (e) => {
                        e.stopPropagation(), e.preventDefault(), P(i), c(ni, !0);
                    }),
                    t([h], r),
                    t([u, l, d, r], i),
                    j && !a.gameTimer && t([i], n);
            },
            di = (n) => {
                const r = pi(),
                    i = e(["lotus", r], "a"),
                    f = e(["icon"]),
                    m = e([p]),
                    l = e(["store"]),
                    u = e(["name"]),
                    d = e([N]),
                    h = e(["close"]),
                    g = e(["x-icon"]),
                    v = [Mn, Sn].includes(r),
                    j = b(In),
                    y = b(Rn);
                s(u, "Lotus: Life Counter"),
                    r === Mn
                        ? (s(d, "Get"),
                          s(l, "App Store"),
                          (i.href = "https://apps.apple.com/app/mtg-life-counter-app-lotus/id1498057193"))
                        : r === Sn &&
                          (s(d, "Install"),
                          s(l, "Google Play"),
                          (i.href = "https://play.google.com/store/apps/details?id=com.vanilla.mtgcounter")),
                    pe && (s(d, "Review"), s(l, "Enjoying Lotus?"), s(u, "Leave a Review")),
                    h.addEventListener(o, (e) => {
                        e.stopPropagation(), e.preventDefault(), P(i), c(pe ? Rn : In, !0);
                    }),
                    t([g], h),
                    t([l, u], m),
                    t([f, m, d, h], i),
                    v && !a.gameTimer && ((!j && !pe) || (!y && pe)) && t([i], n);
            },
            ca = () => {
                if (Kt) {
                    [Te, ce].forEach((e) => ji(e)), (Kt = !1);
                    return;
                }
                if (
                    (Bn.forEach((e) => {
                        n(e, E), e.addEventListener(x, () => e.remove(), w);
                    }),
                    (Bn.length = 0),
                    ce)
                ) {
                    const e = [Te, ce],
                        t = () => {
                            (ce = null),
                                (Te = null),
                                r(z, Ee),
                                setTimeout(() => {
                                    m.querySelectorAll(".fade-out").forEach((e) => e.remove());
                                }, 700),
                                Ae(),
                                ye();
                        },
                        s = () => {
                            Te && Te.remove(), r(z, Ee);
                        },
                        o = () => {
                            ce && ce.remove();
                        };
                    r(z),
                        n(z, Ee),
                        e.forEach((e) => n(e, E)),
                        z.addEventListener(oe, t, w),
                        Te.addEventListener(x, s, w),
                        ce.addEventListener(oe, o, w);
                    return;
                }
                (ce = e(["menu-button-overlay"])), (Te = e(["list"]));
                const h = (n, o) => {
                        const a = e([n]),
                            i = e([N]);
                        return s(i, o), t([i], a), { wrapper: a, btn: i };
                    },
                    R = e(["game-states-wrapper"]),
                    p = e(["monarch-btn"]),
                    v = e(["initiative-btn"]),
                    b = e(["day-night-btn"]),
                    l = e(["planechase-gamemode-btn"]),
                    u = e(["archenemy-gamemode-btn"]),
                    c = e(["bounty-gamemode-btn"]),
                    j = e(["life-history-btn"]),
                    _ = e(["dice-btn"]),
                    C = e(["card-search-btn"]),
                    A = h("high-roll", "High Roll"),
                    S = h("players", "Players"),
                    M = h("restart", "Restart"),
                    D = h("settings", "Settings"),
                    L = h("more", "Help"),
                    P = [A, D, L, S, M],
                    H = 360 / P.length;
                P.forEach((e, t) => {
                    g("--rotation", `${(t + 1) * H}deg`, e.wrapper);
                }),
                    M.btn.addEventListener(o, zt),
                    A.btn.addEventListener(o, () => mi()),
                    S.btn.addEventListener(o, ha),
                    D.btn.addEventListener(o, Ze),
                    L.btn.addEventListener(o, yi),
                    n(z),
                    s(p, "Monarch"),
                    p.addEventListener(o, () => {
                        f(p), z.click(), Hi();
                    }),
                    m.querySelector(".monarch-coin") && n(p),
                    s(v, "Initiative"),
                    v.addEventListener(o, () => {
                        f(v), z.click(), Ii();
                    }),
                    m.querySelector(".initiative-coin") && n(v),
                    s(_, "Dice"),
                    _.addEventListener(o, () => {
                        $o(), z.click();
                    }),
                    s(j, "History"),
                    j.addEventListener(o, () => {
                        Ln(), z.click();
                    }),
                    s(C, "Card Search"),
                    C.addEventListener(o, () => {
                        bi(), z.click();
                    }),
                    s(b, "Day/Night"),
                    b.addEventListener(o, () => {
                        f(b), z.click(), Ta();
                    }),
                    m.querySelector(`.${De}`) && n(b),
                    s(l, "Planechase"),
                    l.addEventListener(o, () => {
                        f(l), z.click(), k.isActive ? rs() : jn() || f(l);
                    }),
                    k.isActive && n(l),
                    s(u, "Archenemy"),
                    u.addEventListener(o, () => {
                        f(u), z.click(), y.isActive ? pn() : wn() || f(u);
                    }),
                    y.isActive && n(u),
                    s(c, "Bounty"),
                    c.addEventListener(o, () => {
                        f(c), z.click(), O.isActive ? Cn() : On() || f(c);
                    }),
                    O.isActive && n(c),
                    t([A.wrapper, D.wrapper, L.wrapper, S.wrapper, M.wrapper], Te),
                    t([Te], z),
                    t([ce], d),
                    t([_, j, C, p, v, b, l, u, c], R),
                    t([R], ce),
                    a.gameTimer && t([T.node], ce),
                    ne.node && ne.node.remove(),
                    di(ce),
                    ui(ce),
                    i("Menu Overlay", F),
                    pe ||
                        (fn(["dice/d4", "dice/d6", "dice/d8", "dice/d10", "dice/d12", "dice/d20", "dice/coin"]),
                        fn(["/images/day.jpg", "/images/night.jpg"], !1));
            },
            jn = () =>
                Gn() ? ((k.isActive = !0), uo(), c(Qe, k), le(), i("Planechase: Enable Planechase"), !0) : (qn(), !1),
            rs = () => {
                (k.isActive = !1), (k.history = []), (k.currentPlane = null);
                const e = m.querySelector(".planechase-btn");
                e && e.remove(), c(Qe, k), le(), i("Planechase: Disable Planechase");
            },
            wn = () =>
                Gn() ? ((y.isActive = !0), Zs(), c(Ke, y), le(), i("Archenemy: Enable Archenemy"), !0) : (qn(), !1),
            pn = () => {
                (y.isActive = !1), (y.history = []), (y.ongoingSchemes = []), (y.currentScheme = null);
                const e = m.querySelector(".archenemy-btn");
                e && e.remove(), c(Ke, y), le(), i("Archenemy: Disable Archenemy");
            },
            On = () => (Gn() ? ((O.isActive = !0), $s(), c(Be, O), le(), i("Bounty: Enable Bounty"), !0) : (qn(), !1)),
            Cn = () => {
                (O.isActive = !1), (O.history = []), (O.currentBounty = null), (O.rewardLevel = 1);
                const e = m.querySelector(".bounty-btn");
                e && e.remove(), c(Be, O), le(), i("Bounty: Disable Bounty");
            },
            hi = async () => {
                z = e(["menu-button"]);
                const s = e(["menu-button-shape"]),
                    i = e(["menu-button-shape-inner"]);
                if (
                    (t([i], s),
                    t([s, e(["menu-button-stroke"])], z),
                    z.addEventListener(o, ca),
                    Kn(),
                    An(Kn),
                    a.whitelabelIcon !== null)
                ) {
                    const e = await se(a.whitelabelIcon);
                    g("--icon", `url(${e})`, z), n(z, "whitelabeled");
                }
                return z;
            },
            mi = () => {
                let o = 0,
                    a = null,
                    r = new Map(),
                    s;
                const c = () => Math.floor(Math.random() * 20) + 1;
                j.forEach((n) => {
                    if (n.alive === !1) return;
                    const l = e(["roll-overlay"]),
                        d = e(["numbers-wrapper"]);
                    let i;
                    do i = c();
                    while (r.has(i));
                    r.set(i, n),
                        S(d, i),
                        n.playerCardInnerClick(),
                        t([d], l),
                        t([l], n.playerCard.children[0]),
                        Bn.push(l),
                        i > o && ((o = i), (a = n), (s = l));
                }),
                    j.forEach((e) => {
                        e === a &&
                            (n(s, "highlight"),
                            co(s),
                            (h.ongoingGame = !0),
                            h.isActive &&
                                X === null &&
                                ((h.currentPlayerIndex = Rs().findIndex((t) => t.name === e.name)),
                                (h.startingPlayerIndex = h.currentPlayerIndex),
                                _o()));
                    }),
                    (Kt = !0),
                    i("High Roll");
            },
            fi = () => {
                ot(d), (d.classList = ""), ii();
            },
            dt = (a, r, c, l = {}) =>
                new Promise((u) => {
                    const m = e(["confirm-overlay"]),
                        v = e(["button-wrapper"]),
                        b = e(["confirm-text"]),
                        p = e([N, "cancel"]),
                        g = e([N, "confirm"]);
                    s(b, a), s(p, r), s(g, c);
                    let h = null;
                    if (l.checkboxLabel) {
                        h = e(["confirm-checkbox"]);
                        const i = e(["confirm-checkbox-box"]),
                            n = e(["confirm-checkbox-label"]);
                        s(n, l.checkboxLabel), h.addEventListener(o, () => f(h)), t([i, n], h);
                    }
                    const j = (e) => (l.checkboxLabel ? { confirmed: e, checked: !!h && _(h) } : e);
                    g.addEventListener(o, () => {
                        u(j(!0));
                    }),
                        p.addEventListener(o, () => {
                            u(j(!1)),
                                n(m, E),
                                m.addEventListener(
                                    x,
                                    () => {
                                        m.remove();
                                    },
                                    w
                                );
                        }),
                        t([p, g], v),
                        t([b, ...(h ? [h] : []), v], m),
                        t([m], d),
                        i("Custom Confirm", F);
                }),
            un = (a, r, c) => {
                const l = e(["confirm-overlay"]),
                    h = e(["button-wrapper"]),
                    m = e(["confirm-text"]),
                    u = e([N, "confirm"]);
                s(m, a),
                    s(u, r),
                    u.addEventListener(o, () => {
                        n(l, E),
                            l.addEventListener(
                                x,
                                () => {
                                    l.remove(), c && c();
                                },
                                w
                            );
                    }),
                    t([u], h),
                    t([m, h], l),
                    t([l], d),
                    i("Custom Alert", F);
            },
            vi = () => {
                let n = 0;
                const o = e(["restart-overlay"]),
                    i = e([p]),
                    a = 3,
                    r = "Restarting",
                    c = () => {
                        (n = (n + 1) % (a + 1)), s(i, r + ".".repeat(n));
                    },
                    l = setInterval(c, 500);
                t([i], o), t([o], d);
            },
            zt = () => {
                const e = j.some((e) => (e.nickname || "").trim());
                dt(
                    "Are you sure you want to restart the game?",
                    "Cancel",
                    "Restart",
                    e ? { checkboxLabel: "Reset player names" } : {}
                ).then((e) => {
                    const t = typeof e == "object" ? e.confirmed : e,
                        n = typeof e == "object" && e.checked;
                    if (t) {
                        He(), (Re.length > 0 || oi() !== null) && cr(), vi();
                        const e = j;
                        n &&
                            e.forEach((e) => {
                                e.nickname = "";
                            }),
                            cn(),
                            (j = $n(e)),
                            (Re = []),
                            (It = 0),
                            (k.history = []),
                            (k.currentPlane = null),
                            (y.history = []),
                            (y.ongoingSchemes = []),
                            (y.currentScheme = null),
                            ba(),
                            Ls(),
                            sn(!1),
                            U(),
                            i("Restart Game"),
                            on();
                    }
                });
            },
            Zo = (e) => {
                e.forEach((e) => {
                    localStorage.removeItem(e);
                });
            },
            Qo = (e) => {
                dt(e, "Cancel", "Restart").then((e) => {
                    e && (Zo([ls, Qt]), on());
                });
            },
            _i = () => {
                let e = performance.now(),
                    n = !1,
                    t = !0;
                const o = () => {
                    (t = !m.hidden), t && (e = performance.now());
                };
                m.addEventListener("visibilitychange", o);
                const s = () => {
                    const o = document.getElementById("life-history-overlay"),
                        a = o !== null;
                    if (t && !a) {
                        const t = performance.now();
                        !n &&
                            t - e > 5e3 &&
                            ((n = !0),
                            i("Possible UI Freeze Detected"),
                            Qo("Possible UI freeze detected. Do you want to restart the game?")),
                            (e = t);
                    }
                    requestAnimationFrame(s);
                };
                requestAnimationFrame(s);
            },
            wi = () => (Nt.innerHTML = Nt.innerHTML.replaceAll(/data-src/g, "src")),
            Oi = () => {
                (Nt = Content), (Content.style = ""), Content.remove();
            },
            xi = () => {
                const e = navigator.userAgent || navigator.vendor || window.opera,
                    t = /android|iPad|iPhone|iPod|windows phone|mobile/i;
                return t.test(e) && !window.MSStream;
            },
            Ci = () => {
                n(Oe, "desktop"), wi(), t([Nt], d);
            },
            Tn = (e, t) => {
                n(e, t),
                    e.addEventListener(
                        x,
                        (n) => {
                            r(e, t), e.removeEventListener(x, n.currentTarget);
                        },
                        w
                    );
            },
            Yo = () => ve.matchMedia("(display-mode: standalone)").matches || ve.navigator.standalone,
            qo = "tutorialOverlay_v1",
            Si = () => b(qo) === !0,
            Mi = () => c(qo, !0),
            Fi = () => {
                if (Si() || Yo()) return;
                const l = e(["first-time-user-overlay"]),
                    y = e(["text-wrapper"]),
                    b = e(["image"]),
                    u = e(["headline"]),
                    h = e([p]),
                    a = e([N]),
                    z = () => {
                        const n = e(["cover-card"]),
                            o = e(["life-count"]),
                            i = e(["side", "minus"]),
                            a = e(["side", "plus"]);
                        return s(o, te), S(i, "-"), S(a, "+"), t([o, i, a], n), n;
                    },
                    c = z(),
                    k = () => {
                        const n = e(["commander-card"]),
                            o = e(["label"]),
                            i = e(["button"]);
                        return (
                            (o.innerHTML = `<b>Commander</b>Damage you’ve received`),
                            s(i, "Return to Game"),
                            t([o, i], n),
                            n
                        );
                    },
                    j = k(),
                    T = () => {
                        const n = e(["options-card"]),
                            o = e(["inner"]),
                            {
                                backgroundEntry: i,
                                backgroundEntryText: a,
                                killEntry: r,
                                killEntryText: c,
                                partnerToggleEntry: l,
                                partnerToggleEntryIcon: u,
                                partnerToggleEntryText: d,
                            } = Ps();
                        return s(a, "Background"), s(c, "Kill"), s(d, "Partner"), t([i, r, l], o), t([o], n), n;
                    },
                    M = T(),
                    A = () => {
                        Tn(a, o),
                            n(l, E),
                            l.addEventListener(
                                x,
                                () => {
                                    l.remove(), Mi();
                                },
                                w
                            ),
                            i("Got it! Close overlay");
                    },
                    C = () => A(),
                    v = () => g("--bg", `var(--${we[Math.floor(Math.random() * we.length)]})`, c),
                    O = () => {
                        c.addEventListener(
                            oe,
                            () => {
                                v(),
                                    setInterval(() => {
                                        v();
                                    }, 1e3);
                            },
                            w
                        ),
                            r(c, "swipe-up"),
                            Tn(a, o),
                            a.removeEventListener(o, O),
                            a.addEventListener(o, C),
                            s(u, "Fullscreen mode"),
                            (h.innerHTML = "Add website to <b>home screen</b> for fullscreen mode"),
                            s(a, "Good to know!"),
                            i("Got it! Fullscreen mode");
                    },
                    _ = () => {
                        c.addEventListener(
                            oe,
                            () => {
                                n(j, Z), setTimeout(() => n(c, "swipe-up"), 500);
                            },
                            w
                        ),
                            r(c, "swipe-left"),
                            Tn(a, o),
                            a.removeEventListener(o, _),
                            pe ? a.addEventListener(o, C) : a.addEventListener(o, O),
                            s(u, "Make it yours"),
                            (h.innerHTML = "Swipe <b>up</b> or <b>down</b> to show player options"),
                            s(a, "Alright!"),
                            i("Got it! Swipe up/down");
                    },
                    m = () => {
                        if (f) return;
                        n(c, "swipe-left"), a.addEventListener(o, _), n(a), (f = !0);
                    };
                let f = !1;
                l.addEventListener(x, () => setTimeout(() => m(), 6e3), w),
                    c.addEventListener(_t, () => m(), w),
                    l.addEventListener(o, () => m(), w),
                    s(u, "Commander damage"),
                    (h.innerHTML = "Swipe <b>left</b> or <b>right</b> to track Commander damage"),
                    s(a, "Got it!"),
                    t([c, j, M], b),
                    t([b, u, h], y),
                    t([y, a], l),
                    t([l], d),
                    i("First Time User Overlay", F);
            },
            Ti = (e) => {
                const t = new URLSearchParams(ve.location.search);
                return t.get(e);
            },
            zi = () => Ti("force") === "true",
            Ko = (e, t) => {
                const n = Math.max(0, Math.min(e.right, t.right) - Math.max(e.left, t.left)),
                    s = Math.max(0, Math.min(e.bottom, t.bottom) - Math.max(e.top, t.top));
                return n * s;
            },
            Ni = (e, t) => {
                const i = e.getBoundingClientRect();
                let o = 0,
                    s = null;
                if (
                    (j.forEach((e) => {
                        const n = e.playerCard,
                            c = n.getBoundingClientRect(),
                            a = Ko(i, c);
                        a > o ? ((o = a), (s = n)) : r(n, t);
                    }),
                    s)
                ) {
                    _(s, t) || n(s, t);
                    const o = { [u]: -90, [l]: 90, [Fe]: 180, [ae]: 0 },
                        i = Array.from(s.classList).find((e) => e in o);
                    if (i) {
                        const n = parseFloat(getComputedStyle(e).getPropertyValue("--rotation")) || 0,
                            s = o[i];
                        let t = s - n;
                        t > 180 ? (t -= 360) : t < -180 && (t += 360), g("--rotation", `${n + t}deg`, e);
                    }
                }
            },
            Li = (e, t) => {
                const n = t.getBoundingClientRect(),
                    s = d.getBoundingClientRect();
                n.top < e && C(t, "top", `${e}px`),
                    n.left < e && C(t, Y, `${e}px`),
                    n.bottom > s.height - e && C(t, "top", `${s.height - n.height - e}px`),
                    n.right > s.width - e && C(t, Y, `${s.width - n.width - e}px`);
            },
            Ri = (e) => {
                const t = parseFloat(getComputedStyle(e).getPropertyValue("--rotation")) || 0,
                    n = Math.floor(Math.random() * 11) + 5,
                    s = Math.random() < 0.5 ? t + n : t - n;
                g("--rotation", `${s}deg`, e);
            },
            Uo = (o) => {
                let b,
                    v,
                    u = !1,
                    c = !1;
                const f = `${o}-coin`,
                    O = `Remove ${o.charAt(0).toUpperCase() + o.slice(1)} Coin`,
                    p = `active-${o}`,
                    g = m.querySelector(`.${f}`),
                    a = e([f]),
                    l = e(["coin-info"]),
                    E = 10,
                    y = "remove-indicator",
                    k = () => {
                        const n = e([y]);
                        s(n, O), t([n], z);
                    },
                    h = () => {
                        const e = z.querySelectorAll(`.${y}`);
                        e.forEach((e) => {
                            n(e, Z), e.addEventListener(x, () => e.remove(), w);
                        });
                    },
                    _ = () => j.forEach((e) => r(e.playerCard, o));
                if (g) {
                    g.remove(), _(), i(`Removed ${o.charAt(0).toUpperCase() + o.slice(1)} Coin`);
                    return;
                }
                s(l, "Drag me"),
                    setTimeout(() => {
                        n(l, Z), l.addEventListener(x, () => l.remove(), w);
                    }, 2e3),
                    t([a, l], d),
                    a.addEventListener(
                        st,
                        (e) => {
                            const t = e.touches[0];
                            (b = t.clientX - a.getBoundingClientRect().left),
                                (v = t.clientY - a.getBoundingClientRect().top),
                                n(a),
                                n(z, p),
                                e.preventDefault();
                        },
                        { passive: !1 }
                    ),
                    a.addEventListener(
                        _t,
                        (e) => {
                            const t = e.touches[0];
                            C(a, "top", `${t.clientY - v}px`),
                                C(a, Y, `${t.clientX - b}px`),
                                C(a, "transition", "transform 0.5s ease-in-out");
                            const n = a.getBoundingClientRect(),
                                s = z.getBoundingClientRect(),
                                i = Ko(n, s);
                            i > 0 ? (c || (k(), (c = !0)), (u = !0)) : (c && (h(), (c = !1)), (u = !1)),
                                Ni(a, o),
                                e.preventDefault();
                        },
                        { passive: !1 }
                    ),
                    a.addEventListener(qe, () => {
                        if ((C(a, "transition", ""), r(a), r(z, p), Ri(a), u)) {
                            _(), h(), a.remove(), i(`Removed ${o.charAt(0).toUpperCase() + o.slice(1)} Coin`);
                            return;
                        }
                        c && (h(), (c = !1)), Li(E, a), i(`${o.charAt(0).toUpperCase() + o.slice(1)} Coin moved`);
                    }),
                    i(`${o.charAt(0).toUpperCase() + o.slice(1)} Coin added`);
            },
            Hi = () => Uo("monarch"),
            Ii = () => Uo("initiative"),
            Dt = {
                d4: [1, 4],
                d6: [1, 6],
                d8: [1, 8],
                d10: [1, 10],
                d12: [1, 12],
                d20: [1, 20],
                custom: [1, 100],
                coin: ["Heads", "Tails"],
            },
            $o = () => {
                const l = e(["dice-overlay"]),
                    v = e(["close-dice-overlay-btn"]),
                    m = e(["rng-list"]),
                    h = e(["pin-entry-btn"]),
                    u = e(["pin-menu-btn"]),
                    j = e(["pin-hint"]);
                let p = !1;
                const g = () => {
                        m.querySelectorAll(".pin-die-badge").forEach((e) => {
                            e.dataset.type === a.pinnedDie ? n(e) : r(e);
                        }),
                            a.pinnedDie === "menu"
                                ? (n(u), s(u, "Unpin dice menu"))
                                : (r(u), s(u, "Pin dice menu to screen")),
                            s(h, a.pinnedDie ? "Change pinned die" : "Pin to main screen"),
                            a.pinnedDie ? n(h, "has-pin") : r(h, "has-pin");
                    },
                    f = (e) => {
                        (p = e), e ? n(l, "pin-picking") : r(l, "pin-picking");
                    };
                Object.keys(Dt).forEach((l) => {
                    const d = e(["roller", l]),
                        u = e(["icon-wrapper"]),
                        y = e(["icon"]),
                        h = e(["label"]),
                        v = () => {
                            const e = u.querySelectorAll(".result");
                            e.forEach((e) => {
                                e.remove();
                            });
                        },
                        j = (e) => {
                            n(d, "rolled"),
                                t([e], u),
                                setTimeout(() => {
                                    n(e, E),
                                        e.addEventListener(x, () => {
                                            e.remove(), r(d, "rolled");
                                        });
                                }, 1e3);
                        };
                    if (l === "custom") {
                        n(d, "custom");
                        const a = e([], M),
                            l = e([], "label"),
                            r = e(["roll-btn"]),
                            h = b(Ro);
                        h !== null ? (a.value = h) : (a.value = 100),
                            (a.id = "custom-dice-input"),
                            (l.htmlFor = "custom-dice-input"),
                            (a.type = "number"),
                            (a.placeholder = 100),
                            s(r, "Roll"),
                            a.addEventListener(M, () => {
                                (a.value = a.value.replace(/\D/g, "")), c(Ro, a.value);
                            }),
                            r.addEventListener(o, () => {
                                v();
                                const t = e(["result"]);
                                S(t, Le(1, a.value)), j(t), i(`Roll Custom Dice: ${a.value}`);
                            }),
                            t([a, l, r], u);
                    } else {
                        n(d, l.startsWith("d") ? "dice" : "coin"),
                            d.addEventListener(o, () => {
                                if (p) {
                                    cs(a.pinnedDie === l ? null : l),
                                        g(),
                                        f(!1),
                                        i(`Pin Die: ${a.pinnedDie || "none"}`);
                                    return;
                                }
                                v();
                                const [c, d] = Dt[l],
                                    o = l === "coin" ? Le(0, 1) : Le(c, d),
                                    r = l === "coin" ? Dt[l][o] : o,
                                    n = e(["result"]);
                                if (l === "coin") {
                                    const o = e(["result-text"]);
                                    s(o, r), t([o], n);
                                } else S(n, r);
                                j(n), i(`Roll ${l}`);
                            });
                        const r = e(["pin-die-badge"]);
                        (r.dataset.type = l), t([r], d);
                    }
                    s(h, l), t([y], u), t([u, h], d), t([d], m);
                }),
                    h.addEventListener(o, () => {
                        f(!0), i("Pin: enter pick mode");
                    }),
                    s(j, "Tap a die to pin it"),
                    u.addEventListener(o, () => {
                        cs(a.pinnedDie === "menu" ? null : "menu"),
                            g(),
                            f(!1),
                            i(`Pin Dice Menu: ${a.pinnedDie === "menu" ? "on" : "off"}`);
                    }),
                    g(),
                    l.addEventListener(o, (e) => {
                        p && (e.target === l || e.target === m) && f(!1);
                    }),
                    v.addEventListener(o, () => {
                        n(l, E),
                            l.addEventListener(
                                x,
                                () => {
                                    l.remove();
                                },
                                w
                            ),
                            i("Close Dice Overlay");
                    }),
                    t([v, j, m, h, u], l),
                    t([l], d),
                    i("Dice Overlay", F);
            },
            He = () => {
                J &&
                    (Re.push({ player: J.player, change: J.totalChange, life: J.finalLife, timestamp: J.timestamp }),
                    J.debounceTimer && clearTimeout(J.debounceTimer),
                    (J = null));
            },
            Bo = (e, t) => {
                const n = j.indexOf(e),
                    s = 2500;
                J && J.player !== n && He(),
                    J
                        ? ((J.totalChange += t), (J.finalLife = e.life))
                        : (J = {
                              player: n,
                              totalChange: t,
                              finalLife: e.life,
                              timestamp: new Date().toLocaleString(),
                              debounceTimer: null,
                          }),
                    J.debounceTimer && clearTimeout(J.debounceTimer),
                    (J.debounceTimer = setTimeout(() => {
                        He();
                    }, s));
            },
            Ln = (a = null) => {
                He();
                const u = a === null,
                    xe = () => (h && h.isActive && h.ongoingGame ? h.currentTurn : null),
                    l = u
                        ? {
                              id: v ? v.id : null,
                              name: v ? v.name : "Current Game",
                              startDate: v ? v.startDate : Date.now(),
                              startingLife: v ? v.startingLife : te,
                              duration: v ? Mt(v.startDate) : 0,
                              playerCount: v ? v.playerCount : A,
                              players: j,
                              history: Re,
                              turnCount: xe(),
                          }
                        : a,
                    f = u ? j : l.players,
                    ye = u ? Re : l.history,
                    ge = l.startingLife,
                    m = e(["life-history-overlay"]),
                    he = e(["history-meta-header"]),
                    K = e(["meta-row", "game-name-row"]),
                    J = e(["meta-label"]);
                s(J, "Game Name");
                const D = e(["game-name"]);
                s(D, l.name),
                    D.addEventListener(o, () => {
                        Yi(l.id, l.name, (e) => {
                            s(D, e);
                        });
                    }),
                    t([J, D], K);
                const fe = e(["meta-row", "date-row"]),
                    Se = e(["meta-icon", "calendar-icon"]),
                    be = e(["meta-value"]);
                s(be, ai(l.startDate)), t([Se, be], fe);
                const W = e(["meta-row", "stats-row"]);
                let z = null,
                    P = null,
                    M = null;
                if (!u) {
                    z = e(["stat-section", "winner-section"]);
                    const a = e(["stat-label"]);
                    s(a, "Winner");
                    const i = e(["stat-value"]);
                    (M = async () => {
                        i.innerHTML = "";
                        const a = R.find((e) => e.id === l.id),
                            o = a ? a.winner : l.winner;
                        if (o != null && f[o]) {
                            const s = e(["winner-icon-card"]),
                                a = f[o],
                                r = await se(a.backgroundImage);
                            g("--bg-color", We(a.background), s),
                                r
                                    ? (g("--bg-image", `var(--gradientOverlay), url(${r})`, s), n(s, "image"))
                                    : $e(a.background) && n(s, "white-avatar");
                            const c = document.createTextNode(a.nickname || a.name);
                            t([s, c], i);
                        } else {
                            const n = e(["set-value-btn"]);
                            s(n, "Set Winner"), t([n], i);
                        }
                        P && P();
                    }),
                        M(),
                        i.addEventListener(o, () => {
                            Xi(l.id, f, M);
                        }),
                        t([a, i], z);
                }
                const pe = e(["stat-section"]),
                    ue = e(["stat-label"]);
                s(ue, "Starting Life");
                const le = e(["stat-value"]);
                (le.innerHTML = `<span class="heart-svg"></span>${ge}`), t([ue, le], pe);
                const ce = e(["stat-section"]),
                    re = e(["stat-label"]);
                s(re, "Game Duration");
                const Y = e(["stat-value"]);
                (Y.innerHTML = `<span class="clock-svg"></span>${yn(u ? Mt(v.startDate) : l.duration)}`),
                    t([re, Y], ce);
                let G = null;
                u &&
                    (G = setInterval(() => {
                        const e = Mt(v.startDate);
                        Y.innerHTML = `<span class="clock-svg"></span>${yn(e)}`;
                    }, 1e3));
                let L = null;
                const I = u ? xe() : l.turnCount;
                if (I != null) {
                    L = e(["stat-section"]);
                    const n = e(["stat-label"]);
                    s(n, "Turns");
                    const o = e(["stat-value"]);
                    (o.innerHTML = `<span class="turns-svg"></span>${I}`), t([n, o], L);
                }
                const N = [];
                z && N.push(z), N.push(pe, ce), L && N.push(L), t(N, W);
                const H = e(["players-section"]),
                    ee = e(["players-label"]);
                s(ee, f.length === 1 ? "Player" : "Players");
                const X = [],
                    oe = e(["players-list"]),
                    ie = [],
                    ke = [...f].reverse();
                ke.forEach(async (i, a) => {
                    const d = f.length - 1 - a,
                        h = e(["player-item"]);
                    ie[d] = h;
                    const c = e(["player-icon-card"]);
                    g("--bg-color", We(i.background), c), $e(i.background) && n(c, "white-avatar");
                    const m = e(["player-name"]),
                        v = i.nickname || i.name;
                    s(m, v), t([c, m], h), t([h], oe);
                    const p = await se(i.backgroundImage);
                    p && (g("--bg-image", `var(--gradientOverlay), url(${p})`, c), n(c, "image"), r(c, "white-avatar")),
                        h.addEventListener(o, () => {
                            Gi(l.id, d, i.nickname, i.name, (e) => {
                                s(m, e || i.name), (f[d].nickname = e), Bs(e), u && (os(j[d]), U());
                                const t = ae(f);
                                f.forEach((e, n) => {
                                    X[n] && s(X[n], t[n]);
                                }),
                                    M && M();
                            });
                        });
                }),
                    t([ee, oe], H),
                    (P = () => {
                        const t = R.find((e) => e.id === l.id),
                            n = t ? t.winner : null;
                        ie.forEach((t, s) => {
                            const o = t.querySelector(".winner-badge");
                            if ((o && o.remove(), n === s)) {
                                const n = e(["winner-badge"]);
                                t.insertBefore(n, t.firstChild);
                            }
                        });
                    }),
                    u || P();
                const q = e(["format-section"]),
                    S = [
                        "commander",
                        "dual-commander",
                        "standard",
                        "modern",
                        "pauper",
                        "legacy",
                        "archon",
                        "australian-highlander",
                        "canadian-highlander",
                        "centurion",
                        "conquest",
                        "co",
                        "dandan",
                        "european-highlander",
                        "leviathan",
                        "oathbreaker",
                        "old-school",
                        "pauper-commander",
                        "pioneer",
                        "predh",
                        "premodern",
                        "primordial",
                        "tiny-leaders",
                        "vintage",
                    ],
                    Q = () => {
                        const e = [];
                        return (
                            k && k.isActive && e.push("planechase"),
                            y && y.isActive && e.push("archenemy"),
                            O && O.isActive && e.push("bounty"),
                            e
                        );
                    },
                    de = ["planechase", "archenemy", "bounty"],
                    $ = () => {
                        if (u) {
                            const e = v && v.gameModes ? v.gameModes : [],
                                t = e.filter((e) => !de.includes(e)),
                                n = Q();
                            return [...t, ...n];
                        }
                        return l.gameModes || [];
                    };
                let b = $();
                const Ce = b.some((e) => !S.includes(e)),
                    Z = e(["format-header"]),
                    V = e(["format-label"]);
                s(V, Ce ? "Format & Variation" : "Format");
                const T = e(["format-list"]),
                    ve = () => {
                        const e = u ? $() : l.gameModes || [];
                        Qi(e, (e) => {
                            if (u) {
                                const t = Q();
                                e.includes("planechase") && !t.includes("planechase")
                                    ? jn()
                                    : !e.includes("planechase") && t.includes("planechase") && rs(),
                                    e.includes("archenemy") && !t.includes("archenemy")
                                        ? wn()
                                        : !e.includes("archenemy") && t.includes("archenemy") && pn(),
                                    e.includes("bounty") && !t.includes("bounty")
                                        ? On()
                                        : !e.includes("bounty") && t.includes("bounty") && Cn();
                                const n = e.filter((e) => !de.includes(e));
                                v && ((v.gameModes = n), c(at, v)), (b = $()), B(b);
                            } else {
                                l.gameModes = e;
                                const t = R.findIndex((e) => e.id === l.id);
                                t !== -1 && ((R[t].gameModes = e), c(je, R)), (b = e), B(b);
                            }
                        });
                    },
                    B = (n) => {
                        const i = n.some((e) => !S.includes(e));
                        if ((s(V, i ? "Format & Variation" : "Format"), (T.innerHTML = ""), n.length > 0)) {
                            const i = [...n].sort((e, t) => {
                                const n = S.includes(e),
                                    s = S.includes(t);
                                return n && !s ? -1 : !n && s ? 1 : 0;
                            });
                            i.forEach((n) => {
                                const a = S.includes(n),
                                    r = a ? "format-tag" : "variation-tag",
                                    i = e([r, `mode-${n}`]),
                                    c = n
                                        .split("-")
                                        .map((e) => e.charAt(0).toUpperCase() + e.slice(1))
                                        .join(" ");
                                s(i, c), i.addEventListener(o, ve), t([i], T);
                            });
                        } else {
                            const n = e(["set-value-btn"]);
                            s(n, "Set Format"), n.addEventListener(o, ve), t([n], T);
                        }
                    };
                t([V], Z), B(b), t([Z, T], q);
                const C = e(["all-games-btn"]);
                s(C, "All Games"),
                    C.addEventListener(o, () => {
                        if (_(m, E)) return;
                        n(m, E);
                        let e = !1;
                        const t = () => {
                                if (e) return;
                                (e = !0), m.remove(), No();
                            },
                            s = (e) => {
                                e.target === m && e.animationName === "fadeOut" && (m.removeEventListener(x, s), t());
                            };
                        m.addEventListener(x, s), setTimeout(t, 350);
                    });
                const _e = u ? [K, W, H] : [K, fe, W, H];
                q && _e.push(q), t(_e, he);
                const we = e(["timeline-label"]);
                s(we, "Game Timeline");
                const Oe = e(["life-history-header"]),
                    p = e(["life-history-timeline"]),
                    me = e(["close-life-history-overlay-btn"]),
                    Ee = () => {
                        const o = e(["life-history-row"]);
                        f.forEach((i) => {
                            const a = e(["life-history-cell"]);
                            g("--bg", We(i.background), a), $e(i.background) && n(a, "white-text"), s(a, ge), t([a], o);
                        }),
                            t([o], p);
                    };
                Ee();
                const ae = (e) => {
                        const t = e.map((e) =>
                            e.nickname ? e.nickname.charAt(0).toUpperCase() : e.name.replace("Player ", "P")
                        );
                        for (let n = 1; n <= 10; n++) {
                            const s = new Map();
                            let o = !1;
                            if (
                                (e.forEach((i, a) => {
                                    if (!i.nickname) return;
                                    const r = t[a];
                                    if (s.has(r)) {
                                        o = !0;
                                        const c = s.get(r);
                                        e[c].nickname && (t[c] = e[c].nickname.substring(0, n + 1).toUpperCase()),
                                            (t[a] = i.nickname.substring(0, n + 1).toUpperCase());
                                    } else s.set(r, a);
                                }),
                                !o)
                            )
                                break;
                        }
                        return t;
                    },
                    Ae = ae(f);
                f.forEach(async (o, i) => {
                    const a = e(["header-item"]);
                    (X[i] = a),
                        s(a, Ae[i]),
                        g("--bg", We(o.background), a),
                        $e(o.background) && n(a, "white-text"),
                        t([a], Oe);
                    const c = await se(o.backgroundImage);
                    c && (g("--bg", `var(--gradientOverlay), url(${c})`, a), n(a, "image"), r(a, "white-text"));
                });
                const ne = (e, t, n) => {
                    if (typeof e == "number") return e === t;
                    const s = parseInt(e, 10);
                    return isNaN(s) ? `Player ${e}` === n : s - 1 === t || s === t;
                };
                if (
                    (ye.forEach((o) => {
                        const i = e(["life-history-row", "change-row"]);
                        f.forEach((a, r) => {
                            const c = e(["life-history-cell", "change-cell"]);
                            if (ne(o.player, r, a.name)) {
                                n(c, "active");
                                const e = o.change > 0 ? "positive" : "negative";
                                n(c, e), g("--bg", We(a.background), c), $e(a.background) && n(c, "white-text");
                                const t = o.change > 0 ? `+${o.change}` : `${o.change}`;
                                s(c, t);
                            }
                            t([c], i);
                        }),
                            t([i], p);
                        const a = e(["life-history-row"]);
                        f.forEach((i, r) => {
                            const c = e(["life-history-cell"]);
                            ne(o.player, r, i.name) &&
                                (g("--bg", We(i.background), c), $e(i.background) && n(c, "white-text"), s(c, o.life)),
                                t([c], a);
                        }),
                            t([a], p);
                    }),
                    ye.length === 0)
                ) {
                    if (u)
                        for (let n = 0; n < 3; n++) {
                            const s = e(["life-history-row"]);
                            f.forEach(() => {
                                const n = e(["life-history-cell", "empty-cell"]);
                                t([n], s);
                            }),
                                t([s], p);
                        }
                    const n = ["empty-timeline-overlay"];
                    u || n.push("past-game");
                    const o = e(n);
                    s(o, u ? "No life total changes so far." : "No life total changes in this game."), t([o], p);
                }
                if (!u) {
                    const i = e(["delete-game-btn"]);
                    s(i, "Delete Game"),
                        i.addEventListener(o, () => {
                            dt("Delete this game from history?", "Cancel", "Delete").then((e) => {
                                e &&
                                    (ta(l.id),
                                    n(m, E),
                                    m.addEventListener(
                                        x,
                                        () => {
                                            m.remove(), No();
                                        },
                                        w
                                    ));
                            });
                        }),
                        t([i], p);
                }
                t([he, we, Oe, p, me, C], m),
                    m.addEventListener("scroll", () => {
                        m.scrollTop > 150 ? n(C, "hidden") : r(C, "hidden");
                    }),
                    me.addEventListener(o, () => {
                        G && clearInterval(G),
                            n(m, E),
                            m.addEventListener(
                                x,
                                () => {
                                    m.remove();
                                },
                                w
                            ),
                            i("Close Life History Overlay");
                    }),
                    t([m], d),
                    i("Life History Overlay", F);
            },
            No = () => {
                const a = e(["all-games-overlay"]),
                    m = e(["close-all-games-overlay"]);
                m.addEventListener(o, () => {
                    n(a, E), a.addEventListener(x, () => a.remove(), w);
                });
                const p = e(["all-games-header"]);
                s(p, "All Games");
                const h = e(["all-games-content"]),
                    g = e(["games-section", "latest"]),
                    b = e(["section-title"]);
                s(b, "Current Game");
                const L = () => {
                        const e = [];
                        return (
                            k && k.isActive && e.push("planechase"),
                            y && y.isActive && e.push("archenemy"),
                            O && O.isActive && e.push("bounty"),
                            e
                        );
                    },
                    N = ["planechase", "archenemy", "bounty"],
                    S = v && v.gameModes ? v.gameModes : [],
                    D = S.filter((e) => !N.includes(e)),
                    M = [...D, ...L()],
                    T = {
                        id: v ? v.id : null,
                        name: v ? v.name : "Current Game",
                        startDate: v ? v.startDate : Date.now(),
                        duration: v ? Mt(v.startDate) : 0,
                        playerCount: v ? v.playerCount : A,
                        startingLife: v ? v.startingLife : te,
                        gameMode: v ? v.gameMode : hn(),
                        gameModes: M,
                        players: j,
                    },
                    z = Do(T, !0, a);
                t([b, z], g), t([g], h);
                let u = null;
                if (R.length > 0) {
                    const o = e(["games-section", "previous"]),
                        i = e(["section-title"]),
                        m = document.createTextNode("Previous Games "),
                        d = e(["section-count"]);
                    s(d, R.length),
                        t([m, d], i),
                        t([i], o),
                        (u = e(["games-list"])),
                        R.forEach((e, s) => {
                            const o = Do(e, !1, a, () => {
                                R.splice(s, 1), c(je, R), o.remove(), R.length === 0 && (n(l, "hidden"), r(a, "edit"));
                            });
                            t([o], u);
                        }),
                        t([u], o),
                        t([o], h);
                }
                const l = e(["edit-games-btn"]);
                s(l, "Edit"),
                    R.length === 0 && n(l, "hidden"),
                    l.addEventListener(o, () => {
                        f(a, "edit"), _(a, "edit") ? s(l, "Done") : s(l, "Edit");
                    });
                const C = e(["all-games-scroll-container"]);
                t([p, h], C), t([C, l, m], a), t([a], d), i("All Games Overlay", F);
            },
            Do = (i, a = !1, r = null, c = null) => {
                const l = e(["game-card"]);
                a && n(l, "current");
                const S = e(["game-card-main"]),
                    k = e(["game-card-name-row"]),
                    O = e(["game-card-name"]);
                s(O, i.name), t([O], k);
                const m = e(["game-card-info-row"]),
                    f = e(["game-card-date"]);
                f.innerHTML = `<span class="calendar-icon"></span>${ai(i.startDate)}`;
                const p = e(["game-card-duration"]);
                (p.innerHTML = `<span class="clock-icon"></span>${yn(i.duration)}`), t([f, p], m);
                const v = e(["game-card-stats-row"]),
                    b = e(["game-card-players"]),
                    j = e(["players-avatars"]);
                for (let n = 0; n < i.playerCount; n++) {
                    const s = e(["player-avatar"]);
                    (s.style.zIndex = i.playerCount - n), t([s], j);
                }
                const y = e(["player-count-number"]);
                s(y, i.playerCount), t([j, y], b);
                const _ = e(["game-card-life"]);
                _.innerHTML = `<span class="heart-icon"></span>${i.startingLife}`;
                let h = null;
                i.turnCount !== null &&
                    i.turnCount !== void 0 &&
                    ((h = e(["game-card-turns"])), (h.innerHTML = `<span class="turns-icon"></span>${i.turnCount}`));
                const C = [b, _];
                h && C.push(h), t(C, v);
                let u = null;
                if (i.gameModes && i.gameModes.length > 0) {
                    u = e(["game-card-format-row"]);
                    const n = [
                            "commander",
                            "dual-commander",
                            "standard",
                            "modern",
                            "pauper",
                            "legacy",
                            "archon",
                            "australian-highlander",
                            "canadian-highlander",
                            "centurion",
                            "conquest",
                            "co",
                            "dandan",
                            "european-highlander",
                            "leviathan",
                            "oathbreaker",
                            "old-school",
                            "pauper-commander",
                            "pioneer",
                            "predh",
                            "premodern",
                            "primordial",
                            "tiny-leaders",
                            "vintage",
                        ],
                        o = [...i.gameModes].sort((e, t) => {
                            const s = n.includes(e),
                                o = n.includes(t);
                            return s && !o ? -1 : !s && o ? 1 : 0;
                        });
                    o.forEach((o) => {
                        const a = n.includes(o),
                            r = a ? "format-tag" : "variation-tag",
                            i = e([r]),
                            c = o
                                .split("-")
                                .map((e) => e.charAt(0).toUpperCase() + e.slice(1))
                                .join(" ");
                        s(i, c), t([i], u);
                    });
                }
                const A = [k, m, v];
                u && A.push(u), t(A, S);
                let d = null;
                if (a) {
                    d = e(["game-card-ongoing"]);
                    const n = e(["ongoing-label"]);
                    s(n, "Ongoing"), t([n], d);
                } else if (i.winner !== null && i.winner !== void 0 && i.players && i.players[i.winner]) {
                    const o = i.players[i.winner];
                    d = e(["game-card-winner"]);
                    const r = e(["winner-name"]);
                    s(r, o.nickname || o.name);
                    const a = e(["winner-icon"]);
                    g("--bg-color", We(o.background), a),
                        (async () => {
                            const e = await se(o.backgroundImage);
                            e
                                ? (g("--bg-image", `var(--gradientOverlay), url(${e})`, a), n(a, "image"))
                                : $e(o.background) && n(a, "white-avatar");
                        })();
                    const c = e(["winner-label"]);
                    s(c, "Winner"), t([c, a, r], d);
                }
                if (!a && c) {
                    const n = e(["delete-game-card"]);
                    n.addEventListener(o, (e) => {
                        e.stopPropagation(), c();
                    }),
                        t([n], l);
                }
                return (
                    t([S], l),
                    d && t([d], l),
                    l.addEventListener(o, () => {
                        r &&
                            (n(r, E),
                            r.addEventListener(
                                x,
                                () => {
                                    r.remove(), a ? Ln() : Ln(i);
                                },
                                w
                            ));
                    }),
                    l
                );
            },
            Yi = (i, a, r) => {
                const l = e(["edit-overlay"]),
                    m = e(["edit-title"]);
                s(m, "Edit Game Name");
                const c = e([], "input");
                (c.type = "text"), (c.value = a), n(c, "edit-input");
                const f = e(["edit-btn-wrapper"]),
                    u = e(["edit-cancel-btn"]);
                s(u, "Cancel");
                const h = e(["edit-save-btn"]);
                s(h, "Save");
                const p = () => {
                    n(l, E), l.addEventListener(x, () => l.remove(), w);
                };
                u.addEventListener(o, p),
                    h.addEventListener(o, () => {
                        const e = c.value.trim();
                        e && e !== a && (Zi(i, "name", e), r(e)), p();
                    }),
                    t([u, h], f),
                    t([m, c, f], l),
                    t([l], d),
                    c.focus(),
                    c.select();
            },
            Gi = (i, a, r, c, l) => {
                const h = e(["edit-overlay"]),
                    p = e(["edit-title"]);
                s(p, "Edit Player Name");
                const u = e([], "input");
                (u.type = "text"), (u.value = r || ""), (u.placeholder = c), n(u, "edit-input");
                const g = e(["edit-btn-wrapper"]),
                    m = e(["edit-cancel-btn"]);
                s(m, "Cancel");
                const f = e(["edit-save-btn"]);
                s(f, "Save");
                const v = () => {
                    n(h, E), h.addEventListener(x, () => h.remove(), w);
                };
                m.addEventListener(o, v);
                const b = (e) => {
                    const t = (e || "").trim();
                    t !== (r || "") && (Ji(i, a, t), l(t)), v();
                };
                f.addEventListener(o, () => b(u.value));
                const j = to(Ws(r || ""), b);
                t([m, f], g), t([p, u, g, ...(j ? [j] : [])], h), t([h], d), u.focus(), u.select();
            },
            Xi = (i, a, l) => {
                const u = e(["edit-overlay", "select-winner-overlay"]),
                    v = e(["edit-title"]);
                s(v, "Select Winner");
                const h = e(["winner-players-list"]),
                    j = [...a].reverse();
                j.forEach(async (c, d) => {
                    const b = a.length - 1 - d,
                        m = e(["winner-player-option"]),
                        u = e(["player-icon-card"]);
                    g("--bg-color", We(c.background), u), $e(c.background) && n(u, "white-avatar");
                    const p = e(["player-name"]);
                    s(p, c.nickname || c.name),
                        t([u, p], m),
                        m.addEventListener(o, () => {
                            ea(i, b), l(), f();
                        }),
                        t([m], h);
                    const v = await se(c.backgroundImage);
                    v && (g("--bg-image", `var(--gradientOverlay), url(${v})`, u), n(u, "image"), r(u, "white-avatar"));
                });
                const m = e(["winner-player-option", "no-winner"]),
                    y = e(["player-icon-card"]),
                    b = e(["player-name"]);
                s(b, "No Winner"),
                    t([y, b], m),
                    m.addEventListener(o, () => {
                        const e = R.find((e) => e.id === i);
                        e && ((e.winner = null), c(je, R)), l(), f();
                    }),
                    t([m], h);
                const f = () => {
                        n(u, E), u.addEventListener(x, () => u.remove(), w);
                    },
                    p = e(["edit-cancel-btn"]);
                s(p, "Cancel"), p.addEventListener(o, f), t([v, h, p], u), t([u], d);
            },
            Qi = (i, a) => {
                const h = e(["edit-game-modes-overlay"]),
                    T = [
                        { id: "commander", name: "Commander" },
                        { id: "dual-commander", name: "Dual Commander" },
                        { id: "standard", name: "Standard" },
                        { id: "modern", name: "Modern" },
                        { id: "pauper", name: "Pauper" },
                        { id: "legacy", name: "Legacy" },
                    ],
                    g = [
                        { id: "archon", name: "Archon" },
                        { id: "australian-highlander", name: "Australian Highlander" },
                        { id: "canadian-highlander", name: "Canadian Highlander" },
                        { id: "centurion", name: "Centurion" },
                        { id: "conquest", name: "Conquest" },
                        { id: "co", name: "CO" },
                        { id: "dandan", name: "Dandân" },
                        { id: "european-highlander", name: "European Highlander" },
                        { id: "leviathan", name: "Leviathan" },
                        { id: "oathbreaker", name: "Oathbreaker" },
                        { id: "old-school", name: "Old School" },
                        { id: "pauper-commander", name: "Pauper Commander" },
                        { id: "pioneer", name: "Pioneer" },
                        { id: "predh", name: "PreDH" },
                        { id: "premodern", name: "Premodern" },
                        { id: "primordial", name: "Primordial" },
                        { id: "tiny-leaders", name: "Tiny Leaders" },
                        { id: "vintage", name: "Vintage" },
                    ],
                    F = [
                        { id: "cedh", name: "cEDH" },
                        { id: "treachery", name: "Treachery" },
                        { id: "shogun", name: "Shogun" },
                        { id: "two-headed-giant", name: "Two-Headed Giant" },
                        { id: "three-headed-giant", name: "Three-Headed Giant" },
                        { id: "planechase", name: "Planechase" },
                        { id: "archenemy", name: "Archenemy" },
                        { id: "bounty", name: "Bounty" },
                    ],
                    b = [
                        { id: "planechase", name: "Planechase" },
                        { id: "two-headed-giant", name: "Two-Headed Giant" },
                        { id: "three-headed-giant", name: "Three-Headed Giant" },
                        { id: "archenemy", name: "Archenemy" },
                    ],
                    S = [...T, ...g].map((e) => e.id),
                    A = (i || []).find((e) => S.includes(e)) || null,
                    D = new Set((i || []).filter((e) => !S.includes(e)));
                let l = A,
                    c = new Set(D),
                    f = g.some((e) => e.id === A);
                const O = e(["format-overlay-content"]),
                    _ = e(["format-overlay-title"]);
                s(_, "Format");
                const u = e(["format-list"]),
                    y = new Map(),
                    N = (e) => {
                        const t = e === "commander" ? F : b;
                        return new Set(t.map((e) => e.id));
                    },
                    L = () => {
                        y.forEach((e, t) => {
                            e.forEach((e) => {
                                const s = e.dataset.variationId;
                                l === t
                                    ? (r(e, "hidden"), c.has(s) ? n(e, "active") : r(e, "active"))
                                    : (n(e, "hidden"), r(e, "active"));
                            });
                        });
                    },
                    C = (i) => {
                        const a = e(["format-entry"]);
                        l === i.id && n(a, "active");
                        const d = e();
                        return (
                            s(d, i.name),
                            a.addEventListener(o, () => {
                                if (
                                    (u.querySelectorAll(".format-entry.active").forEach((e) => r(e, "active")),
                                    l === i.id)
                                )
                                    (l = null), c.clear();
                                else {
                                    (l = i.id), n(a, "active");
                                    const e = N(i.id);
                                    c = new Set([...c].filter((t) => e.has(t)));
                                }
                                L();
                            }),
                            t([d], a),
                            a
                        );
                    },
                    k = (i, a, d = !1) => {
                        const u = e(["variation-entry", "subentry"]);
                        (u.dataset.variationId = i.id),
                            d && n(u, "last-subentry"),
                            l !== a && n(u, "hidden"),
                            c.has(i.id) && n(u, "active");
                        const h = e();
                        return (
                            s(h, i.name),
                            u.addEventListener(o, () => {
                                c.has(i.id) ? (c.delete(i.id), r(u, "active")) : (c.add(i.id), n(u, "active"));
                            }),
                            t([h], u),
                            u
                        );
                    };
                T.forEach((e) => {
                    const o = C(e);
                    t([o], u);
                    const n = e.id === "commander" ? F : b,
                        s = [];
                    n.forEach((o, i) => {
                        const r = i === n.length - 1,
                            a = k(o, e.id, r);
                        s.push(a), t([a], u);
                    }),
                        y.set(e.id, s);
                });
                const m = e(["view-all-btn"]);
                f && n(m, "hidden"), s(m, "View all formats"), t([m], u);
                const j = [];
                g.forEach((e) => {
                    const s = C(e);
                    f || n(s, "hidden-format"), j.push(s), t([s], u);
                    const o = b,
                        i = [];
                    o.forEach((s, a) => {
                        const c = a === o.length - 1,
                            r = k(s, e.id, c);
                        f || n(r, "hidden-format"), j.push(r), i.push(r), t([r], u);
                    }),
                        y.set(e.id, i);
                }),
                    m.addEventListener(o, () => {
                        (f = !0), n(m, "hidden"), j.forEach((e) => r(e, "hidden-format"));
                    }),
                    t([_, u], O);
                const M = e(["edit-btn-wrapper"]),
                    v = e(["edit-cancel-btn"]);
                s(v, "Cancel");
                const p = e(["edit-save-btn"]);
                s(p, "Confirm");
                const z = () => {
                    n(h, E), h.addEventListener(x, () => h.remove(), w);
                };
                v.addEventListener(o, z),
                    p.addEventListener(o, () => {
                        const e = [];
                        l && e.push(l), c.forEach((t) => e.push(t)), a(e), z();
                    }),
                    t([v, p], M),
                    t([O, M], h),
                    t([h], d);
            },
            Zi = (e, t, n) => {
                if (v && v.id === e) {
                    (v[t] = n), c(at, v);
                    return;
                }
                const s = R.findIndex((t) => t.id === e);
                s !== -1 && ((R[s][t] = n), c(je, R));
            },
            Ji = (e, t, n) => {
                if (v && v.id === e) {
                    (j[t].nickname = n || ""), c(Qt, j);
                    return;
                }
                const s = R.findIndex((t) => t.id === e);
                s !== -1 && ((R[s].players[t].nickname = n || ""), c(je, R));
            },
            ea = (e, t) => {
                const s = R.findIndex((t) => t.id === e);
                if (s === -1) return;
                const n = R[s],
                    o = n.winner;
                o === t
                    ? ((n.winner = null), n.players.forEach((e) => (e.isWinner = !1)))
                    : ((n.winner = t), n.players.forEach((e, n) => (e.isWinner = n === t))),
                    c(je, R);
            },
            ta = (e) => {
                (R = R.filter((t) => t.id !== e)), c(je, R), i("Delete Game from History");
            },
            rn = (e, t, s) => {
                e.addEventListener(o, () => {
                    n(t, E),
                        t.addEventListener(
                            x,
                            () => {
                                t.remove();
                            },
                            w
                        ),
                        i(`Close ${s} Overlay`);
                });
            },
            sa = (e) => {
                const t = e.map((e) => ({
                    id: e.id,
                    image: e.image_uris.large,
                    text: e.oracle_text,
                    set: e.set,
                    set_name: e.set_name,
                    type_line: e.type_line,
                }));
                return t;
            },
            zo = async (e) => {
                try {
                    const t = await fetch(`${dn}q=layout%3A${e}&unique=cards`),
                        n = await t.json();
                    return sa(n.data);
                } catch {}
            },
            an = async (e, t) => {
                const n = b(e),
                    o = 24 * 60 * 60 * 1e3,
                    s = new Date();
                if (n) {
                    const i = new Date(n.timestamp);
                    if (s - i > o) {
                        const n = await zo(t);
                        return c(e, { data: n, timestamp: s }), n;
                    }
                    return n.data;
                } else {
                    const n = await zo(t);
                    return c(e, { data: n, timestamp: s }), n;
                }
            },
            aa = (e) => {
                const t = e.map((e) => {
                    const t = e.card_faces?.[0]?.image_uris?.large || e.image_uris?.large,
                        n = e.card_faces?.[0]?.oracle_text || e.oracle_text;
                    return {
                        id: e.id,
                        image: t,
                        text: n,
                        set: e.set,
                        set_name: e.set_name,
                        type_line: e.type_line,
                        name: e.card_faces?.[0]?.name || e.name,
                    };
                });
                return t;
            },
            Fo = async () => {
                try {
                    const e = await fetch(`${dn}q=name%3A%2Fbounty%3A%2F&unique=cards`),
                        t = await e.json();
                    return aa(t.data) || [];
                } catch (e) {
                    return console.error("Failed to load bounty cards:", e), [];
                }
            },
            So = async () => {
                const e = b(us),
                    n = 24 * 60 * 60 * 1e3,
                    t = new Date();
                if (e && e.data && e.data.length > 0) {
                    const s = new Date(e.timestamp);
                    if (t - s > n) {
                        const n = await Fo();
                        return n && n.length > 0 ? (c(us, { data: n, timestamp: t }), n) : e.data;
                    }
                    return e.data;
                } else {
                    const e = await Fo();
                    return e && e.length > 0 && c(us, { data: e, timestamp: t }), e || [];
                }
            },
            vt = (e) => ((O.currentBounty = e), c(Be, O), O.history[e]),
            Lt = (e) => {
                O = b(Be);
                const t = O.history;
                if (e === Ve) return t.length > 0 ? vt(O.currentBounty) : Lt(Me);
                if (e === rt && t.length > 0) {
                    const e = O.currentBounty;
                    if (e > 0) {
                        const t = e - 1;
                        return (O.currentBounty = t), vt(t);
                    }
                    return vt(e);
                }
                if (e === Me) {
                    if (!Ce || Ce.length === 0) return console.error("No bounty data available"), null;
                    const s = O.currentBounty;
                    if (s !== -1 && s < t.length - 1) {
                        const e = s + 1;
                        return (O.currentBounty = e), vt(e);
                    }
                    let e = Ce.filter((e) => !t.some((t) => t.id === e.id));
                    const n = b(qt);
                    if (n) {
                        const t = Ce.some((e) => e.id in n);
                        t && (e = e.filter((e) => n[e.id] !== !1));
                    }
                    if (e.length === 0) {
                        const s = t.reduce((e, t) => ((e[t.id] = (e[t.id] || 0) + 1), e), {}),
                            o = Math.max(...Object.values(s), 0),
                            i = o + 1;
                        if (((e = Ce.filter((e) => (s[e.id] || 0) < i)), n)) {
                            const t = Ce.some((e) => e.id in n);
                            t && (e = e.filter((e) => n[e.id] !== !1));
                        }
                    }
                    const i = e[Math.floor(Math.random() * e.length)];
                    O.history.push(i);
                    const o = t.length - 1;
                    return (O.currentBounty = o), vt(o);
                }
                return null;
            },
            xt = (e) => ((k.currentPlane = e), c(Qe, k), k.history[e]),
            tn = (e) => {
                k = b(Qe);
                const t = k.history;
                if (e === Ve) return t.length > 0 ? xt(k.currentPlane) : tn(Me);
                if (e === rt && t.length > 0) {
                    const e = k.currentPlane;
                    if (e > 0) {
                        const t = e - 1;
                        return (k.currentPlane = t), xt(t);
                    }
                    return xt(e);
                }
                if (e === Me) {
                    const s = k.currentPlane;
                    if (s !== -1 && s < t.length - 1) {
                        const e = s + 1;
                        return (k.currentPlane = e), xt(e);
                    }
                    let e = Ot.filter((e) => !t.some((t) => t.id === e.id));
                    e = e.filter((e) => e.set !== "da1");
                    const n = b(Bt);
                    if (n) {
                        const t = Ot.some((e) => e.id in n);
                        t && (e = e.filter((e) => n[e.id] !== !1));
                    }
                    if (e.length === 0) {
                        const s = t.reduce((e, t) => ((e[t.id] = (e[t.id] || 0) + 1), e), {}),
                            o = Math.max(...Object.values(s), 0),
                            i = o + 1;
                        if (((e = Ot.filter((e) => (s[e.id] || 0) < i)), n)) {
                            const t = Ot.some((e) => e.id in n);
                            t && (e = e.filter((e) => n[e.id] !== !1));
                        }
                    }
                    const i = e[Math.floor(Math.random() * e.length)];
                    k.history.push(i);
                    const o = t.length - 1;
                    return (k.currentPlane = o), xt(o);
                }
                return null;
            },
            ma = () => {
                const e = Array(4).fill("no effect").concat(["planeswalk", "chaos"]),
                    t = Math.floor(Math.random() * e.length);
                return e[t];
            },
            Co = () => {
                const a = e(["planar-die-overlay"]),
                    r = ma(),
                    c = e(["result", r.replace(" ", "-")]),
                    l = m.querySelector(".planechase-overlay");
                if ((s(c, r), t([c], a), r === "planeswalk")) {
                    const n = e(["planeswalk-btn"]);
                    n.addEventListener(o, async () => {
                        l || (await Zn()), m.querySelector(".btn.planeswalk").click();
                    }),
                        s(n, "Planeswalk"),
                        t([n], a);
                }
                if (r === "chaos" && !l) {
                    const n = e(["chaos-btn"]);
                    n.addEventListener(o, Zn), s(n, "View card"), t([n], a);
                }
                a.addEventListener(o, () => {
                    n(a, E),
                        a.addEventListener(x, () => {
                            a.remove();
                        }),
                        i("Planechase: Close Planar Die Overlay");
                }),
                    t([a], d),
                    i("Planar Die Overlay", F);
            },
            Xn = (e) => {
                const t = "https://svgs.scryfall.io/card-symbols/",
                    n = /\{([^}]*)\}/gm,
                    s = `<span class="symbol $1"><img src="${t}$1.svg" width="1" height="1"></span>`;
                return e.replace(/\n/g, "<br><br>").replace(n, s);
            },
            Qn = (n, a) => {
                n.addEventListener(o, () => {
                    const n = e(["game-mode-info-overlay"]),
                        o = e(["close"]),
                        r = e(["game-mode-info-content"]),
                        c = {
                            Archenemy: `<h2>Archenemy</h2><p>The Archenemy deck includes all schemes and is shuffled before each game and when depleted.</p><p><b>Rules:</b> One player (the archenemy) faces a team of opponents (the heroes). The archenemy starts with 40 life, goes first, and controls the scheme deck. The heroes each start with 20 life. The archenemy wins by eliminating all opponents, while the heroes win if the archenemy is defeated.</p><p>At the start of the archenemy's precombat main phase, they reveal and activate a scheme card. Schemes are placed on the bottom of the scheme deck when resolved or abandoned.</p>`,
                            Planechase: Xn(
                                `<h2>Planechase</h2><p>The Planechase deck includes all available planes and reshuffles automatically when depleted, ensuring continuous gameplay.</p><p><b>Rules:</b> During your turn, you may roll the planar die whenever you could cast a sorcery. The first roll is free; each additional roll costs {1} more than the last. If you roll {PW}, planeswalk to a new Plane. If you roll {CHAOS}, trigger the Plane's chaos ability. Blank faces do nothing.</p><p><b>Tip:</b> Hold the Planechase button to instantly roll the Planar die.</p>`
                            ),
                            Bounty: `<h2>Bounty</h2><p>The Bounty deck is a shared deck of bounty cards. The app reshuffles automatically when depleted.</p><p><b>Rules:</b> Starting on turn 3, a bounty card is revealed. Each bounty has a condition — meet it during your turn to claim the reward!</p><p>After a bounty is claimed, a new one is revealed next turn. If unclaimed, the reward level increases (up to Level 4).</p>`,
                        };
                    (r.innerHTML = c[a]),
                        s(o, "Got it!"),
                        rn(o, n, "Game Mode Info"),
                        t([r, o], n),
                        t([n], d),
                        i(`${a}: Show Info`),
                        i(`${a} Info Overlay`, F);
                });
            },
            Zn = async () => {
                if (((Ot = await an(_s, "planar")), m.querySelector(".planechase-overlay"))) return;
                const A = b(Bt);
                if (A) {
                    const e = Object.values(A).some((e) => e === !0);
                    if (!e) {
                        un("You need to enable some cards to play Planechase", "Edit Cards", () => {
                            Ze(),
                                setTimeout(() => {
                                    const e = m.querySelector(".edit-planechase-cards");
                                    e && e.click();
                                }, 100);
                        });
                        return;
                    }
                }
                const l = e(["planechase-overlay", "game-mode-overlay"]),
                    h = e(["close-planechase-overlay-btn", "overlay-close-btn"]),
                    p = e(["planechase-headline", "overlay-headline"]),
                    w = e(["card-wrapper"]),
                    a = e(["planechase-card"]),
                    C = e(["card-text-wrapper"]),
                    x = e(["card-text"]),
                    O = e(["planechase-controls"]),
                    c = e([N, "previous-plane", I]),
                    y = e([N, "planeswalk"]),
                    j = e([N, "planar-dice"]),
                    u = e([N, "flip-card"]),
                    E = e(["settings", "overlay-settings-btn"]),
                    v = (e) => (x.innerHTML = Xn(e.text)),
                    S = tn(Ve),
                    M = () => {
                        k.currentPlane > 0 ? r(c, I) : n(c, I);
                    };
                c.addEventListener(o, () => {
                    if (_(c, I)) return;
                    const e = tn(rt);
                    e && (g("--bg", `url(${e.image})`, a), v(e), M()), i("Planechase: Previous Plane");
                }),
                    y.addEventListener(o, () => {
                        const e = tn(Me);
                        g("--bg", `url(${e.image})`, a), v(e), r(c, I), i("Planechase: Planeswalk");
                    }),
                    u.addEventListener(o, () => {
                        const e = parseInt(getComputedStyle(Oe).getPropertyValue("--planeFlip").trim(), 10),
                            t = l.offsetWidth >= a.offsetHeight ? e - 90 : e === 0 ? -180 : 0;
                        g("--planeFlip", `${t}deg`, Oe), i("Planechase: Flip Card");
                    }),
                    a.addEventListener(o, () => {
                        f(a, "show-text"), i("Planechase: Show Oracle Text");
                    }),
                    j.addEventListener(o, () => {
                        Co(), i("Planechase: Planar Die");
                    }),
                    E.addEventListener(o, () => {
                        h.click(), Ze(), i("Planechase: Open Settings");
                    }),
                    rn(h, l, "Planechase"),
                    Qn(p, "Planechase"),
                    g("--bg", `url(${S.image})`, a),
                    M(),
                    s(p, "Planechase"),
                    s(c, "Back"),
                    s(y, "Planeswalk"),
                    s(j, "Planar Die"),
                    s(u, "Flip"),
                    v(S),
                    t([x], C),
                    t([C], a),
                    t([a], w),
                    t([c, y, j, u], O),
                    t([h, E, p, w, O], l),
                    t([l], d),
                    l.offsetWidth >= a.offsetHeight && g("--degLabel", '"90"', u),
                    i("Planechase Overlay", F);
            },
            uo = () => {
                if (m.querySelector(".planechase-btn") || !k.isActive) return;
                let s,
                    o = !1;
                const n = e(["planechase-btn"]),
                    a = (e) => {
                        e.preventDefault(),
                            (o = !1),
                            (s = setTimeout(() => {
                                (o = !0), Co(), i("Planechase: Long Press Planechase Button");
                            }, 500));
                    },
                    r = (e) => {
                        e.preventDefault(), clearTimeout(s), o || (Zn(), i("Planechase: Click Planechase Button"));
                    },
                    c = () => clearTimeout(s);
                n.addEventListener(st, a), n.addEventListener(qe, r), n.addEventListener(zs, c), le(), t([n], ge);
            },
            ms = (n, s) => {
                n.addEventListener(o, () => {
                    const n = e(["magnify-overlay"]),
                        o = e(["magnify-card"]),
                        a = s === Ve ? y.history[y.currentScheme] : s;
                    rn(n, n, "Magnified Card"),
                        g("--bg", `url(${a.image})`, o),
                        t([o], n),
                        t([n], d),
                        i("Magnify Card"),
                        i("Magnify Card Overlay", F);
                });
            },
            ya = (e) => y.ongoingSchemes.some((t) => t.id === e.id),
            _a = (e) => {
                e.type_line === "Ongoing Scheme" && !ya(e) && y.ongoingSchemes.push(e);
            },
            wa = (e) => {
                (y.ongoingSchemes = y.ongoingSchemes.filter((t) => t.id !== e.id)), c(Ke, y);
            },
            kt = (e) => {
                y.currentScheme = e;
                const t = y.history[e];
                return _a(t), c(Ke, y), t;
            },
            Xt = (e) => {
                y = b(Ke);
                const t = y.history;
                if (e === Ve) return t.length > 0 ? kt(y.currentScheme) : Xt(Me);
                if (e === rt && t.length > 0) {
                    const e = y.currentScheme;
                    if (e > 0) {
                        const t = e - 1;
                        return (y.currentScheme = t), kt(t);
                    }
                    return kt(e);
                }
                if (e === Me) {
                    const s = y.currentScheme;
                    if (s !== -1 && s < t.length - 1) {
                        const e = s + 1;
                        return (y.currentScheme = e), kt(e);
                    }
                    let e = gt.filter((e) => !t.some((t) => t.id === e.id));
                    const n = b(Wt);
                    if (n) {
                        const t = gt.some((e) => e.id in n);
                        t && (e = e.filter((e) => n[e.id] !== !1));
                    }
                    if (e.length === 0) {
                        const s = t.reduce((e, t) => ((e[t.id] = (e[t.id] || 0) + 1), e), {}),
                            o = Math.max(...Object.values(s), 0),
                            i = o + 1;
                        if (((e = gt.filter((e) => (s[e.id] || 0) < i)), n)) {
                            const t = gt.some((e) => e.id in n);
                            t && (e = e.filter((e) => n[e.id] !== !1));
                        }
                    }
                    const i = e[Math.floor(Math.random() * e.length)];
                    y.history.push(i);
                    const o = t.length - 1;
                    return (y.currentScheme = o), kt(o);
                }
                return null;
            },
            Ca = async () => {
                if (((gt = await an(fs, "scheme")), m.querySelector(".archenemy-overlay"))) return;
                const A = b(Wt);
                if (A) {
                    const e = Object.values(A).some((e) => e === !0);
                    if (!e) {
                        un("You need to enable some cards to play Archenemy", "Edit Cards", () => {
                            Ze(),
                                setTimeout(() => {
                                    const e = m.querySelector(".edit-archenemy-cards");
                                    e && e.click();
                                }, 100);
                        });
                        return;
                    }
                }
                const u = e(["archenemy-overlay", "game-mode-overlay"]),
                    h = e(["close-archenemy-overlay-btn", "overlay-close-btn"]),
                    T = e(["settings", "overlay-settings-btn"]),
                    p = e(["archenemy-headline", "overlay-headline"]),
                    w = e(["cards-wrapper"]),
                    k = e(["schemes-wrapper"]),
                    E = e(["schemes-label"]),
                    c = e(["schemes-carousel"]),
                    M = e(["current-scheme-card-wrapper"]),
                    l = e(["current-scheme-card"]),
                    C = e(["archenemy-controls"]),
                    a = e([N, "previous-scheme", I]),
                    j = e([N, "flip-card"]),
                    v = e([N, "next-scheme"]),
                    P = Xt(Ve),
                    O = () => {
                        if (y.ongoingSchemes.length === 0) return;
                        c.scrollTo({ left: c.scrollWidth, behavior: "smooth" });
                    },
                    f = () => {
                        if (
                            (c.querySelectorAll(".empty-state").forEach((e) => e.remove()),
                            y.ongoingSchemes.length !== 0)
                        )
                            return;
                        const n = e(["empty-state"]),
                            o = e(["empty-state-text"]);
                        s(o, "No ongoing schemes"), t([o], n), t([n], c);
                    },
                    z = () => {
                        y.currentScheme > 0 ? r(a, I) : n(a, I);
                    },
                    L = () => {
                        y.ongoingSchemes.forEach((e) => D(e));
                    },
                    D = (s) => {
                        const a = e(["scheme-card"]),
                            r = e(["abandon-scheme-btn"]);
                        (a.dataset.id = s.id),
                            g("--bg", `url(${s.image})`, a),
                            ms(a, s),
                            f(),
                            r.addEventListener(o, (e) => {
                                e.stopPropagation(),
                                    n(a, "pop-out"),
                                    a.addEventListener(x, () => a.remove()),
                                    wa(s),
                                    f(),
                                    i("Archenemy: Abandon Scheme");
                            }),
                            t([r], a),
                            t([a], c);
                    },
                    R = (e) => c.querySelector(`[data-id='${e.id}']`),
                    S = (e) => {
                        e.type_line === "Ongoing Scheme" &&
                            !R(e) &&
                            (D(e),
                            setTimeout(() => {
                                O();
                            }, 500));
                    };
                v.addEventListener(o, () => {
                    const e = Xt(Me);
                    S(e), g("--bg", `url(${e.image})`, l), r(a, I), i("Archenemy: Next Scheme");
                }),
                    a.addEventListener(o, () => {
                        if (_(a, I)) return;
                        const e = Xt(rt);
                        e && (S(e), g("--bg", `url(${e.image})`, l), z()), i("Archenemy: Previous Scheme");
                    }),
                    j.addEventListener(o, () => {
                        const e = parseInt(getComputedStyle(Oe).getPropertyValue("--archenemy-rotation").trim(), 10),
                            t = e === 0 ? -180 : 0;
                        g("--archenemy-rotation", `${t}deg`, Oe), i("Archenemy: Flip Card");
                    }),
                    T.addEventListener(o, () => {
                        h.click(), Ze(), i("Archenemy: Open Settings");
                    }),
                    ms(l, Ve),
                    rn(h, u, "Archenemy"),
                    Qn(p, "Archenemy"),
                    g("--bg", `url(${P.image})`, l),
                    z(),
                    L(),
                    s(p, "Archenemy"),
                    s(a, "Back"),
                    s(j, "Flip"),
                    s(v, "Next Scheme"),
                    s(E, "Ongoing Schemes"),
                    t([E, c], k),
                    t([l], M),
                    t([k, M], w),
                    t([a, j, v], C),
                    t([h, T, p, w, C], u),
                    t([u], d),
                    i("Archenemy Overlay", F),
                    f(),
                    O();
            },
            Ea = async () => {
                if (((Ce = await So()), m.querySelector(".bounty-overlay"))) return;
                if (!Ce || Ce.length === 0) {
                    un("Could not load bounty cards. Please check your internet connection and try again.", "OK");
                    return;
                }
                const z = b(qt);
                if (z) {
                    const e = Object.values(z).some((e) => e === !0);
                    if (!e) {
                        un("You need to enable some cards to play Bounty", "Edit Cards", () => {
                            Ze(),
                                setTimeout(() => {
                                    const e = m.querySelector(".edit-bounty-cards");
                                    e && e.click();
                                }, 100);
                        });
                        return;
                    }
                }
                const a = e(["bounty-overlay", "game-mode-overlay"]),
                    L = e(["close-bounty-overlay-btn", "overlay-close-btn"]),
                    D = e(["settings", "overlay-settings-btn"]),
                    j = e(["bounty-headline", "overlay-headline"]),
                    C = e(["card-wrapper"]),
                    l = e(["bounty-card"]),
                    T = e(["card-text-wrapper"]),
                    M = e(["card-text"]),
                    A = e(["reward-level-wrapper"]),
                    k = e(["reward-level-label"]),
                    h = e(["reward-levels-display"]),
                    S = e(["bounty-controls"]),
                    u = e([N, "previous-bounty", I]),
                    _ = e([N, "claim-bounty"]),
                    y = e([N, "flip-card"]),
                    R = Lt(Ve),
                    P = [
                        { level: 1, text: "1 Treasure" },
                        { level: 2, text: "2 Treasures" },
                        { level: 3, text: "2 Treasures OR 1 Card" },
                        { level: 4, text: "2 Treasures & 1 Card" },
                    ],
                    v = () => {
                        (h.innerHTML = ""),
                            P.forEach((a) => {
                                const r = e(["reward-level-item"]);
                                a.level === O.rewardLevel && n(r, tt);
                                const l = e(["level-number"]),
                                    d = e(["level-reward"]);
                                s(l, `Level ${a.level}`),
                                    s(d, a.text),
                                    t([l, d], r),
                                    t([r], h),
                                    r.addEventListener(o, () => {
                                        (O.rewardLevel = a.level), c(Be, O), v(), i(`Bounty: Set Level ${a.level}`);
                                    });
                            });
                    },
                    H = () => {
                        O.currentBounty > 0 ? r(u, I) : n(u, I);
                    },
                    B = (e) => (M.innerHTML = Xn(e.text)),
                    p = (e) => {
                        if (!e) return;
                        g("--bg", `url(${e.image})`, l), B(e), H();
                    };
                l.addEventListener(o, () => {
                    f(l, "show-text");
                }),
                    p(R),
                    v(),
                    s(j, "Bounty"),
                    s(k, "Reward Levels"),
                    s(u, "Previous"),
                    s(_, "Claim"),
                    s(y, "Flip"),
                    L.addEventListener(o, () => {
                        n(a, E), a.addEventListener(x, () => a.remove(), w);
                    }),
                    D.addEventListener(o, () => {
                        Ze(), i("Bounty: Click Settings");
                    }),
                    u.addEventListener(o, () => {
                        const e = Lt(rt);
                        p(e), i("Bounty: Previous Bounty");
                    }),
                    _.addEventListener(o, () => {
                        (O.rewardLevel = 1), c(Be, O);
                        const e = Lt(Me);
                        p(e), v(), i("Bounty: Claim Bounty");
                    }),
                    y.addEventListener(o, () => {
                        const e = parseInt(getComputedStyle(Oe).getPropertyValue("--bounty-rotation").trim(), 10),
                            t = e === 0 ? -180 : 0;
                        g("--bounty-rotation", `${t}deg`, Oe), i("Bounty: Flip Card");
                    }),
                    Qn(j, "Bounty"),
                    t([M], T),
                    t([T], l),
                    t([l], C),
                    t([k, h], A),
                    t([u, _, y], S),
                    t([L, D, j, C, A, S], a),
                    t([a], d),
                    i("Bounty Overlay", F);
            },
            Zs = () => {
                if (m.querySelector(".archenemy-btn") || !y.isActive) return;
                const n = e(["archenemy-btn"]);
                n.addEventListener(o, () => {
                    Ca(), i("Archenemy: Click Archenemy Button");
                }),
                    le(),
                    t([n], ge);
            },
            $s = () => {
                if (m.querySelector(".bounty-btn") || !O.isActive) return;
                const n = e(["bounty-btn"]);
                n.addEventListener(o, () => {
                    Ea(), i("Bounty: Click Bounty Button");
                }),
                    le(),
                    t([n], ge);
            },
            Sa = () => {
                const n = e(["other-buttons-wrapper"]);
                (yt = e(["game-timer-wrapper"])), (ge = e(["other-buttons"])), t([yt], ge), t([ge], n), t([n], d);
            },
            Ns = (s) => {
                const i = e(["day-night-overlay", s]),
                    a = e(["image"]),
                    c = e(["label"]),
                    r = () => {
                        n(i, E), i.addEventListener(x, () => i.remove(), w);
                    };
                setTimeout(() => r(), 2e3), i.addEventListener(o, () => r()), t([c], a), t([a], i), t([i], d);
            },
            De = "day-night-switcher",
            Ta = () => {
                const s = m.querySelector(`.${De}`),
                    n = e([De]);
                if (s) {
                    s.remove(), Ae(), ye(), i("Day Night Switcher removed");
                    return;
                }
                a.pinnedDie && cs(null),
                    n.addEventListener(o, () => {
                        f(n, "night"), i("Day Night Switcher: Click"), Ns(_(n, "night") ? "night" : "day");
                    }),
                    t([n], ge),
                    le(),
                    Ns("day"),
                    Ae(),
                    ye(),
                    i("Day Night Switcher added");
            },
            en = "pinned-die-btn",
            lr = ["d4", "d6", "d8", "d10", "d12", "d20", "coin"],
            Na = (s, o) => {
                const c = s === "coin",
                    [d, u] = Dt[s],
                    l = c ? Le(0, 1) : Le(d, u);
                if (c) {
                    Ua(Dt[s][l]), i(`Roll Pinned ${s}`);
                    return;
                }
                o.querySelectorAll(".result").forEach((e) => e.remove());
                const a = e(["result"]);
                S(a, l),
                    n(o, "rolled"),
                    t([a], o),
                    setTimeout(() => {
                        n(a, E),
                            a.addEventListener(
                                x,
                                () => {
                                    a.remove(), r(o, "rolled");
                                },
                                w
                            );
                    }, 1e3),
                    i(`Roll Pinned ${s}`);
            },
            As = () => {
                const r = m.querySelector(`.${en}`);
                r && r.remove();
                const s = a.pinnedDie;
                if (!s) {
                    le();
                    return;
                }
                const c = m.querySelector(`.${De}`);
                c && c.remove();
                const n = e([en, `${en}-${s}`]);
                t([e(["icon"])], n),
                    s === "menu"
                        ? n.addEventListener(o, () => {
                              $o(), i("Pinned Dice Menu: Open");
                          })
                        : n.addEventListener(o, () => Na(s, n)),
                    t([n], ge),
                    le();
            },
            cs = (e) => {
                (a.pinnedDie = e), c(L, a), As();
            },
            ws = "turnTrackerHintOverlay_v1",
            Ha = () => b(ws) === !0,
            Ia = () => c(ws, !0),
            Ba = () => {
                if (Ha()) return;
                const r = e(["turn-tracker-hint-overlay"]),
                    a = e(["close"]),
                    c = e([p]),
                    u = () => {
                        n(r, E),
                            r.addEventListener(
                                x,
                                () => {
                                    r.remove();
                                },
                                w
                            ),
                            Ia(),
                            i("Close Turn Tracker Hint Overlay - Step 2");
                    },
                    l = () => {
                        a.removeEventListener(o, l),
                            a.addEventListener(o, u),
                            s(c, "When at Turn 1 on the starting player, long press to change who starts first"),
                            s(a, "Got it!"),
                            i("Turn Tracker Hint Overlay - Step 2", F);
                    };
                s(c, "Long press the turn tracker to move it back to the previous player if you missclicked"),
                    s(a, "Next"),
                    a.addEventListener(o, l),
                    t([c, a], r),
                    t([r], d),
                    i("Turn Tracker Hint Overlay - Step 1", F);
            },
            ys = (a, r = !1) => {
                const c = e(["turn-change-overlay"]),
                    l = e([p]),
                    u = () => {
                        n(c, E),
                            c.addEventListener(
                                x,
                                () => {
                                    c.remove();
                                },
                                w
                            );
                    },
                    h = setTimeout(u, 1500);
                s(l, r ? `Back to Turn ${a}` : `Turn ${a}`),
                    t([l], c),
                    t([c], d),
                    c.addEventListener(o, () => {
                        clearTimeout(h), u();
                    }),
                    i(`Turn Change Overlay: ${a}${r ? " (Revert)" : ""}`);
            },
            $a = () => {
                const a = e(["turn-change-overlay"]),
                    r = e([p]),
                    c = () => {
                        n(a, E),
                            a.addEventListener(
                                x,
                                () => {
                                    a.remove();
                                },
                                w
                            );
                    },
                    l = setTimeout(c, 1500);
                s(r, "New Starting Player"),
                    t([r], a),
                    t([a], d),
                    a.addEventListener(o, () => {
                        clearTimeout(l), c();
                    }),
                    i("Starting Player Change Overlay");
            };
        let St = null;
        const Ua = (o) => {
                if (!St || !St.overlay.isConnected) {
                    const n = e(["coin-flip-overlay"]),
                        s = e([p]);
                    t([s], n), t([n], d), (St = { overlay: n, text: s, hideTimer: null, removeTimer: null });
                }
                const a = St;
                clearTimeout(a.hideTimer),
                    clearTimeout(a.removeTimer),
                    r(a.overlay, E),
                    s(a.text, o),
                    (a.text.style.animation = "none"),
                    void a.text.offsetWidth,
                    (a.text.style.animation = ""),
                    (a.hideTimer = setTimeout(() => {
                        n(a.overlay, E),
                            (a.removeTimer = setTimeout(() => {
                                a.overlay.remove(), (St = null);
                            }, 300));
                    }, 1500)),
                    i(`Coin Flip Overlay: ${o}`);
            },
            gs = () => {
                (h.turnTimer.duration = 0), (h.turnTimer.countDown = []);
            },
            Ls = () => {
                (h.ongoingGame = !1),
                    (h.currentPlayerIndex = 0),
                    (h.startingPlayerIndex = null),
                    (h.currentTurn = 1),
                    gs();
            },
            Rs = () => G[`p${A}`][$].dir.map((e) => j[e]),
            Hs = () => {
                if (!X) return;
                ie[h.currentPlayerIndex].alive || X.click();
            },
            Yt = () => {
                if (!X) return;
                const i = () => {
                    const n = e(["minutes-seconds"]),
                        i = Math.floor(h.turnTimer.duration / 60),
                        o = h.turnTimer.duration % 60;
                    s(X, h.currentTurn), s(n, `${i}:${o < 10 ? "0" : ""}${o}`), t([n], X);
                };
                s(X, `Turn ${h.currentTurn}`),
                    h.turnTimer.isActive &&
                        (i(),
                        Pe && clearInterval(Pe),
                        (Pe = setInterval(() => {
                            h.turnTimer.duration++, i(), c(Ie, h);
                        }, 1e3)));
                const o = ie[h.currentPlayerIndex],
                    a = j.indexOf(o);
                (X.className = "turn-time-tracker"),
                    a !== -1 && n(X, `ttt-p${a + 1}`),
                    j.forEach((e) => {
                        e.playerCard && r(e.playerCard, "has-turn-tracker");
                    }),
                    n(o.playerCard, "has-turn-tracker"),
                    t([X], ie[h.currentPlayerIndex].playerCard.children[0]),
                    requestAnimationFrame(() => {
                        requestAnimationFrame(() => {
                            const e = X.offsetWidth;
                            g("--turn-tracker-width", `${e}px`, o.playerCard);
                        });
                    }),
                    c(Ie, h);
            },
            Ut = () => {
                Pe && clearInterval(Pe), Yt();
            },
            _o = () => {
                (ie = Rs()), (X = e(["turn-time-tracker"]));
                const t = (e) => {
                        let t = e;
                        for (; !ie[t].alive; ) if (((t = (t + 1) % ie.length), t === e)) break;
                        return t;
                    },
                    a = (e) => {
                        let t = e;
                        for (; !ie[t].alive; ) if (((t = (t - 1 + ie.length) % ie.length), t === e)) break;
                        return t;
                    };
                (h.currentPlayerIndex = t(h.currentPlayerIndex)),
                    Yt(),
                    Ut(),
                    X.addEventListener(o, () => {
                        if (Kt) return;
                        const e = h.currentPlayerIndex;
                        h.currentPlayerIndex = t((h.currentPlayerIndex + 1) % ie.length);
                        const n = t(h.startingPlayerIndex);
                        h.currentPlayerIndex === n &&
                            ie[e].alive &&
                            (h.currentTurn++, i(`Turn ${h.currentTurn}`), ys(h.currentTurn)),
                            (h.turnTimer.duration = 0),
                            Yt(),
                            Ut(),
                            Ba();
                    });
                let n;
                const r = () => {
                        n = setTimeout(() => {
                            const e = h.currentPlayerIndex,
                                n = h.currentTurn;
                            h.currentPlayerIndex = a((h.currentPlayerIndex - 1 + ie.length) % ie.length);
                            const s = t(h.startingPlayerIndex);
                            e === s &&
                                ie[e].alive &&
                                n > 1 &&
                                (h.currentTurn--, i(`Turn ${h.currentTurn}`), ys(h.currentTurn, !0)),
                                n === 1 &&
                                    e === h.startingPlayerIndex &&
                                    ((h.startingPlayerIndex = h.currentPlayerIndex),
                                    c(Ie, h),
                                    $a(),
                                    i("Manual Starting Player Change")),
                                (h.turnTimer.duration = 0),
                                Yt(),
                                Ut();
                        }, 1e3);
                    },
                    s = () => {
                        clearTimeout(n);
                    };
                X.addEventListener(st, r),
                    X.addEventListener(qe, s),
                    X.addEventListener(zs, s),
                    i("Turn and Time Tracker added");
            },
            Yn = () => {
                A !== 1 && (z.click(), m.querySelector(".high-roll .btn").click());
            },
            er = () => {
                let e = null;
                const t = async () => {
                    try {
                        (e = await navigator.wakeLock.request("screen")),
                            e.addEventListener("release", async () => {
                                await t();
                            });
                    } catch {}
                };
                "wakeLock" in navigator &&
                    (t(),
                    document.addEventListener("visibilitychange", () => {
                        e !== null && document.visibilityState === "visible" && t();
                    })),
                    window.addEventListener("unload", () => {
                        e !== null && (e.release(), (e = null));
                    });
            },
            ko = "countersOnPlayerCardHintOverlay_v1",
            nr = () => b(ko) === !0,
            ei = () => c(ko, !0),
            or = () => {
                if (nr()) return;
                const r = e(["show-counters-hint-overlay"]),
                    u = e([p]),
                    h = e(["btn-wrapper"]),
                    c = e([N, "confirm"]),
                    l = e([N, "disable"]),
                    f = m.querySelector(".player-card .counters-on-card"),
                    g = () => {
                        j.forEach((e) => {
                            ue(e), me(e);
                        }),
                            n(r, E),
                            r.addEventListener(
                                x,
                                () => {
                                    r.remove();
                                },
                                w
                            ),
                            ei(),
                            i("Close Counters on Player Card Hint Overlay");
                    };
                if (
                    (c.addEventListener(o, () => {
                        (a.showCountersOnPlayerCard = !0), U(), g();
                    }),
                    l.addEventListener(o, () => {
                        (a.showCountersOnPlayerCard = !1), U(), g();
                    }),
                    (u.innerHTML =
                        "Display counters on player cards? <br><span>(You can toggle this in the settings)<span>"),
                    s(c, "Yes"),
                    s(l, "No"),
                    f)
                )
                    try {
                        const e = f.parentElement.parentElement.cloneNode(!0),
                            n = e.children[0],
                            s = n.querySelector(".killed-overlay");
                        (e.innerHTML = ""),
                            (e.className = "player-card"),
                            (n.className = "player-card-inner"),
                            (n.style = ""),
                            s && s.remove(),
                            t([n], e),
                            t([e], r);
                    } catch {}
                else return;
                t([l, c], h), t([u, h], r), t([r], d), i("Show Counters on Player Card Hint Overlay", F);
            },
            ir = (s, c, l) => {
                const u = "counter-controls",
                    _ = s.playerCard.querySelectorAll(`.${u}`),
                    d = e([u]),
                    b = e([u + "-display"]),
                    p = e([u + "-number"]),
                    k = c.startsWith("tax-") ? "tax" : c,
                    j = e([u + "-icon", k]),
                    v = e([N, "decrease"]),
                    g = e([N, "increase"]),
                    A = () => {
                        d.remove(), r(h(), pt);
                    },
                    M = () => n(d, "soon-remove"),
                    f = () => {
                        clearTimeout(O),
                            clearTimeout(x),
                            r(d, "soon-remove"),
                            (O = setTimeout(A, 3e3)),
                            (x = setTimeout(M, 1500));
                    },
                    y = (e) => {
                        const t = pt;
                        n(e, t), e.addEventListener(oe, () => r(e, t), w);
                    },
                    h = () => m.getElementById(si(s.name + c)) || l,
                    F = () =>
                        Array.from(s.playerCard.querySelectorAll(".counters-on-card .counter")).filter(
                            (e) => e !== h()
                        );
                let O,
                    x,
                    C = !0;
                if (
                    (s.partnerCommander && n(j, c),
                    F().forEach((e) => r(e, pt)),
                    (d.dataset.counterType = c),
                    v.addEventListener(o, () => {
                        const e = s.counters[c];
                        if (a.keepZeroCountersOnPlayerCard && e === 0) {
                            const e = s.playerCard.children[0];
                            Ds(s, c, e, !1), ue(s), d.remove(), i("Remove Zero Counter from Card");
                        } else
                            Pt(-1, h(), s, [c], !0),
                                S(p, s.counters[c]),
                                i("Decrease Counter"),
                                f(),
                                s.counters[c] === 0 && ue(s),
                                y(v);
                    }),
                    g.addEventListener(o, () => {
                        const e = s.counters[c];
                        Pt(1, h(), s, [c], !0), S(p, s.counters[c]), i("Increase Counter"), f(), e === 0 && ue(s), y(g);
                    }),
                    S(v, "-"),
                    S(g, "+"),
                    S(p, s.counters[c]),
                    b.addEventListener(o, () => {
                        f(),
                            Js({
                                initialValue: s.counters[c] ?? 0,
                                confirmText: "Set",
                                onConfirm: (e) => {
                                    let t = Math.max(e, 0);
                                    c === "speed" && (t = Math.min(t, 4)),
                                        c.includes("tax") && (t = Math.round(t / 2) * 2),
                                        (s.counters = s.counters ?? {}),
                                        (s.counters[c] = t),
                                        S(p, t),
                                        ue(s),
                                        U(),
                                        c === "poison" && t >= 10 && ft(s, "poison"),
                                        i("Set Counter Manually"),
                                        f();
                                },
                            });
                    }),
                    _.length > 0 &&
                        _.forEach((e) => {
                            e.dataset.counterType === c
                                ? (n(e, E), e.addEventListener(oe, () => e.remove(), w), (C = !1), r(h(), pt))
                                : e.remove();
                        }),
                    !C)
                )
                    return;
                t([p, j], b), t([v, b, g], d), t([d], s.playerCard.children[0]), f();
            },
            ue = (i) => {
                const r = "counters-on-card",
                    c = i.playerCard.querySelector(`.${r}:not(.commander-damage-counters)`),
                    l = e([r]);
                if ((c && c.remove(), !a.showCountersOnPlayerCard || !a.showRegularCounters)) return;
                const d = (e, t) => t !== 0 || a.keepZeroCountersOnPlayerCard || Ms(i).includes(e);
                if (!i.counters || !Object.entries(i.counters).some(([e, t]) => d(e, t))) return;
                Object.entries(i.counters).forEach(([a, r]) => {
                    if (!d(a, r)) return;
                    const c = e(["counter", a.startsWith("tax-") ? "tax" : a]);
                    s(c, i.counters[a]),
                        i.partnerCommander && n(c, a),
                        r === 0 && n(c, "zero-counter"),
                        t([c], l),
                        (c.id = si(i.name + a)),
                        c.addEventListener(o, () => {
                            ir(i, a, c), n(c, pt);
                        });
                }),
                    t([l], i.playerCard.children[0]);
            },
            me = async (i) => {
                const r = "commander-damage-counters",
                    c = i.playerCard.querySelector(`.counters-on-card.${r}`),
                    l = e(["counters-on-card", r]);
                if ((c && c.remove(), !a.showCountersOnPlayerCard || !a.showCommanderDamageCounters)) return;
                if (!i.commanderDamage || i.commanderDamage.length === 0) return;
                let d = !1;
                for (const c of i.commanderDamage) {
                    const u = c.player,
                        r = j.find((e) => e.name === u);
                    if (!r) continue;
                    for (const [h, m] of Object.entries(c.damage)) {
                        if (m === 0) continue;
                        d = !0;
                        const u = e(["counter", "commander-damage-counter"]);
                        if ((s(u, m), r.backgroundImage)) {
                            {
                                const e = await se(h === "commander1" ? r.backgroundImage : r.backgroundImagePartner);
                                if (e)
                                    g("--cmdDmgBg", `var(--gradientOverlay), url(${e})`, u),
                                        n(u, "has-background-image"),
                                        n(u, "white-text");
                                else if (r.background) {
                                    const e = !we.includes(r.background);
                                    g("--cmdDmgBg", e ? r.background : `var(--${r.background})`, u),
                                        e && !gn(r.background) && n(u, "white-text");
                                }
                            }
                        } else if (r.background) {
                            const e = !we.includes(r.background);
                            g("--cmdDmgBg", e ? r.background : `var(--${r.background})`, u),
                                e && !gn(r.background) && n(u, "white-text");
                        }
                        r.partnerCommander && h === "commander2" && n(u, "partner-commander"),
                            a.clickableCommanderDamageCounters &&
                                u.addEventListener(o, () => {
                                    i.triggerSwipe(ee);
                                }),
                            t([u], l);
                    }
                }
                d && t([l], i.playerCard.children[0]);
            },
            si = (e) => {
                let t = e.replace(/[^a-zA-Z0-9-_:.]/g, "-");
                return /^[a-zA-Z]/.test(t) || (t = "id-" + t), t;
            },
            Da = () => {
                localStorage.clear(),
                    indexedDB.databases().then((e) => {
                        e.forEach((e) => {
                            indexedDB.deleteDatabase(e.name);
                        });
                    });
            },
            ii = async () => {
                sr(),
                    U(),
                    a.setLifeByTappingNumber && n(d, "set-life-by-tap-enabled"),
                    a.clickableCommanderDamageCounters && n(d, "clickable-cmd-dmg-enabled"),
                    Xo(),
                    Go(),
                    Vo(),
                    ua(d, [G[`p${A}`][$].key, `${A}-players`]),
                    j.forEach((n, s) => {
                        if (G[`p${A}`][$].colors[s] === !1) {
                            const n = e(["empty-player-card"]);
                            t([n], d);
                        }
                        const o = pa(n, s);
                        (j[s].playerCard = o), bs(n), ue(n), me(n), it(n), t([o], d);
                    }),
                    Ts(),
                    sn();
                const s = await hi();
                t([s], d),
                    Sa(),
                    uo(),
                    Zs(),
                    $s(),
                    As(),
                    le(),
                    Ae(),
                    pe && a.showClockOnMainScreen && (Vn(), ye()),
                    Zt(),
                    h.isActive && (h.ongoingGame ? _o() : h.autoHighroll && Yn()),
                    h.autoHighroll && !h.ongoingGame && Yn();
            };
        m.addEventListener("DOMContentLoaded", () => {
            if (((d = m.body), Yo() && n(Oe, "standalone-mode"), Oi(), !xi() && !zi())) {
                Ci();
                return;
            }
            zn(),
                An(() => zn()),
                Ka(),
                ii(),
                fn(ar),
                Fi(),
                ve.addEventListener("orientationchange", () => {
                    fi(), zn(), Kn();
                }),
                m.addEventListener(
                    "dblclick",
                    (e) => {
                        e.preventDefault();
                    },
                    { passive: !1 }
                ),
                m.addEventListener("contextmenu", (e) => {
                    e.preventDefault();
                }),
                _i(),
                (ve.onerror = (e, t, n, s, o) => (
                    console.error("An error occurred:", e, "at", t, n + ":" + s, o),
                    i(e + " @ " + n + ":" + s, "exception", "JavaScript Error"),
                    Qo("Oops! Looks like an overloaded Cyclonic Rift bounced some code! Restart the game?"),
                    !0
                )),
                er();
        });
    })();
